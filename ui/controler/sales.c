/*
 * =====================================================================================
 *
 *       Filename:  sales.c
 *
 *    Description:  
 *
 *        Created:  23.08.10
 *       Revision:  
 *       Compiler:  GCC 4.4
 *
 *         Author:  Yang Zhang, treblih.divad@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#include	"event.h"
#include	"glue.h"
#include	"stack.h"

#define		FIELD_CNT	8
#define		SURE_INVOCATION 19881014

enum FIELDS {
	CLR_ALL,
	CLR_ANY,
	CANCEL,
	DECODE,
	DISCOUNT,
	TOTAL,
	CHARGE,
	SURE,
};

/* func declaration */
static void *clr_all();
static void *clr_any(FORM *);
static void *clr_one(FORM *);
static void *decode(FORM *);
static void *field_input(FORM *);
static void *sure(FORM *);
static void *discount(FORM *, double);
static void *total(FORM *, double);
static void *charge(FORM *, double);

static void *clr_core(struct elem *);
static void *input_core(FORM *, FUNCP);
static void *var_reset();
static void *delete(MENU *);
static void *field_update(FIELD *, double);
static void *menu_update();
static void *restore_table_menu();
static void *update_table_bill();
static int any_more(FORM *);


/* local variables */
static FUNCP event[] = {
	clr_all,
	clr_any,
	clr_one,
	decode,
	field_input,
	field_input,
	field_input,
	sure,

	/* 8 9 10 */
	discount,
	total,
	charge
};

static FUNCP kb_response[] = {
	form_direct,
	form_enter,
	form_backspace,
	form_input
};

static int in_out;
static int plus;
static int mins;

	/*------------------------------------------------
	 *  during the process we hold 2 data structure:
	 *  1. char ** -- res -- idx
	 *  2. stack_t -- stk -- stk->ptr
	 *
	 *  so stk/idx both can indicate
	 *------------------------------------------------*/
static FIELD **fields;
static MENU *menu;
static ITEM **items;
static char **res;
static stack_t *stk;
static WIDGET *widget_bill;

static int stocks_benchmark;
static int idx;
static double price_total;
static double cost_total;
static double disc;
static double before_dis;
static double receive_total;
static double change;
static struct elem info;


/* local func definition */
static void *var_reset()
{
	/* no stocks_benchmark */
	idx = 0;
	price_total = 0.0;
	cost_total = 0.0;
	disc = 0.0;
	before_dis = 0.0;
	info.id = 0;
	*info.name = '\0';
	info.price = 0.0;
	info.cost = 0.0;
	info.stocks = 0;
	info.qty = 0;
	return NULL;
}

static void *clr_all(FORM *form, int is_sure)
{
	if (any_more(form)) {
		return NULL;
	}
	if (is_sure == SURE_INVOCATION) {
		/* SURE invocation */
		stack_reset(stk);
	} else {
		/* user invocation */
		restore_table_menu();
	}
	int tmp = idx;
	for (; idx;) {
		*res[--idx] = '\0';
	}
	/* now idx == 0 */
	/* fields clear */
	set_field_buffer(fields[DISCOUNT], 0, "");
	set_field_buffer(fields[TOTAL], 0, "");
	set_field_buffer(fields[CHARGE], 0, "");

	/* display reset */
	menu_update();
	set_current_field(form, fields[DECODE]);
	/* pos_form_cursor(form); */
	var_reset();
	return NULL;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  clr_any
 *  Description:  move focus to mid menu
 *  		  invocate delete to remove one & reset the data
 *  		  clr_any once -- delete multy
 * =====================================================================================
 */
static void *clr_any(FORM *form)
{
	/* free in sales */
	widget_bill = widget_init(get_win(W_MID), menu,
			     (FUNCP)unpost_menu, 
			     (FUNCP)free_menu,
			     (FUNCP)free_item,
			     get_kb_response_menu(), DESC_NO);
	curs_set(0);
	interact(widget_bill);
	curs_set(1);
	pos_form_cursor(form);
	/* free in clr_all() */
	return NULL;
}

static void *clr_one(FORM *form)
{
	if (any_more(form)) {
		return NULL;
	}
	/* not [--idx], for the sake of distinguish in menu_update() */
	*res[idx] = '\0';
	clr_core(stack_pop(stk));

	/* display reset */
	menu_update();
	--idx;
	pos_form_cursor(form);	/* back focus to form of w_right */
	return NULL;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  delete
 *  Description:  as a userptr of every items in mid menu
 *  		  called by clr_any
 *  		  clr_any once -- delete multy
 * =====================================================================================
 */
static void *delete(MENU *menu)
{
	/* inconvenient to use any_more(form), no form because */
	if (!idx) {
		return NULL;
	}
	int i = item_index(current_item(menu));
	struct elem *del;
	del = stack_get_one(stk, i);
	clr_core(del);

	stack_delete_one(stk, i);
	int current = i;
	/* display reset */
	while (*res[i + 1]) {
		strcpy(res[i], res[i + 1]);	
		++i;
	}
	*res[i] = '\0';
	--idx;
	menu_update();

	/*-----------------------------------------------------------------------------
	 *  menu_update() will generate a new menu, so current item will always be 
	 *  the 1st,
	 *  i don't want this, make it as if it was successive
	 *-----------------------------------------------------------------------------*/
	set_current_item(menu, items[current]);
	return NULL;
}

static void *clr_core(struct elem *del)
{
	int qty = del->qty;
	char sql[100];
	sprintf(sql, 
		"update menu set stocks = stocks %c %d, acc = acc %c %d \
		 where id = %d",
		plus, qty, mins, qty, del->id);
	sqlite3_exec(get_db_main(), sql, 0, 0, 0);

	/*-----------------------------------------------------------------------------
	 *  if clr any/one after discount...
	 *  make it usual as no discount happened
	 *-----------------------------------------------------------------------------*/
	disc = 0.0;
	receive_total = 0.0;
	change = 0.0;
	before_dis -= (del->qty * del->price);
	price_total = before_dis;
	cost_total  -= (del->qty * del->cost);

	/* fields reset */
	field_update(fields[TOTAL], price_total);
	set_field_buffer(fields[DISCOUNT], 0, "");
	set_field_buffer(fields[CHARGE], 0, "");
	wrefresh(get_win(W_RIGHT));
	return NULL;
}

static void *decode(FORM *form)
{
	int qty;
	char code[5] = {0};
	int id;
	char sql[100];
	if (E_INVALID_FIELD == form_validation(form)) {
		return NULL;
	}
	/* clear w_notice */
	print_notice("");
	const FIELD *field = current_field(form);
	memcpy(code, field_buffer(field, 0), 4);
	/* 2013 = 201 * 3 */
	id = atoi(code);
	info.id  = id / 10;
	info.qty = qty = id % 10;
	sprintf(sql, 
	       "select name, price, cost, stocks from menu where id = %d",
		info.id);

	/* info.name == &info.name */
	db_select_1_row(get_db_main(), sql, 4,
			 SELECT_TEXT, info.name,
			 SELECT_DOUBLE, &info.price,
			 SELECT_DOUBLE, &info.cost,
			 SELECT_INT, &info.stocks);
	if (!*info.name) {
		print_notice(" 没有代号为%d的菜，请检查输入", info.id);
		form_driver(form, REQ_CLR_FIELD);
		return NULL;
	}
	/* WARNING, maybe not enough */
	if (info.stocks < info.qty) {
		print_notice(" 强烈警告，%s库存仅%d份，可能不够！\n"
			     " 可能是数据未及时更新，所以我不会否决该笔交易", 
			     info.name, info.stocks);	
	}
	sprintf(sql, 
		"update menu set stocks = stocks %c %d, acc = acc %c %d \
		 where id = %d",
		mins, qty, plus, qty, info.id);
	sqlite3_exec(get_db_main(), sql, 0, 0, 0);

	form_driver(form, REQ_CLR_FIELD);
	before_dis += (info.price * info.qty);
	price_total = before_dis;
	cost_total += (info.cost * info.qty);
	static int acc_num = ITEM_NUM;
	
	if (idx == acc_num) {
		/* expand the items, + NUL */
		ITEM **items_bak = items;
		items = calloc(acc_num + ITEM_NUM + 1, sizeof (ITEM *));
		memcpy(items, items_bak, acc_num * sizeof (ITEM *));
		free(items_bak);

		/* expand the res */
		char **res_bak = res;
		res = bulk_expand(res_bak, acc_num, ITEM_SIZE, ITEM_NUM);
		free(res_bak);

		acc_num += ITEM_NUM;
	}

	/* store */
	/* print_notice("%p", &info); */
	stack_push(stk, &info);
	sprintf(res[idx], "%d份 单%4.1f 总%5.1f  %-16s", 
		info.qty, info.price, info.qty * info.price, info.name);
	field_update(fields[TOTAL], price_total);
	set_field_buffer(fields[DISCOUNT], 0, "");
	set_field_buffer(fields[CHARGE], 0, "");
	++idx;
	menu_update();

	/*-----------------------------------------------------------------------------
	 * name is static, make *name == NUL
	 * see if (!*name)
	 *-----------------------------------------------------------------------------*/
	info.name[0] = '\0';
	return NULL;
}

static void *field_input(FORM *form)
{
	int i = field_index(current_field(form));
	input_core(form, event[DISCOUNT + i]);
	return NULL;
}

static void *input_core(FORM *form, FUNCP callback)
{
	if (E_INVALID_FIELD == form_validation(form)) {
		return NULL;
	}
	const FIELD *field = current_field(form);
	double data = atof(field_buffer(field, 0));
	if (data) {
		if (!callback(form, data)) {
			form_driver(form, REQ_DOWN_FIELD);
			form_driver(form, REQ_END_LINE);
		}
	}
	return NULL;
}

static void *discount(FORM *form, double data)
{
	int check = data;
	if (check > 10 || data < 5) {
		print_notice(" 不可高于10折或低于5折");
		return NULL;
	}
	disc = data;
	data /= 10.0;
	price_total = before_dis * data;
	field_update(fields[TOTAL], price_total);
	print_notice(" 折前%.1f，折后%.1f，实惠看得见！！", before_dis, price_total);
	return NULL;
}

static void *total(FORM *form, double data)
{
	price_total = data;
	return NULL;
}

static void *charge(FORM *form, double data)
{
	if (data < price_total)	 {
		print_notice(" 他给的钱不够。。。");
		form_driver(form, REQ_CLR_FIELD);
		return (void *)1;
	}
	receive_total = data;
	change        = data - price_total;
	print_notice(" 找零%.1f", change);
	return NULL;
}

static void *sure(FORM *form)
{
	if (!idx) {
		print_notice(" 还没买菜就急着付钱。。。");
		return NULL;
	}

	/* update_table_menu(); */
	update_table_bill();
	list2file(res);
	clr_all(form, SURE_INVOCATION);
	set_current_field(form, fields[DECODE]);
	return NULL;
}

static void *field_update(FIELD *field, double x)
{
	/* update fields-total */
	char tmp[6] = {0};	/* xxx.x + '\0' */
	sprintf(tmp, "%.1f", x);
	set_field_buffer(field, 0, tmp);
	return NULL;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  menu_update
 *  Description:  1 time 1 item
 *  		  idx == idx_old + 1	decode()
 *  		  idx == idx_old	clr_one()
 *  		  idx == idx_old - 1	delete() -- by clr_any()
 *  		  idx == 0		clr_all()
 * =====================================================================================
 */
static void *menu_update()
{
	static int idx_old;
	int i;
	WINDOW *w_mid = get_win(W_MID);
	/* update menu in the mid window */
	if (idx_old) {
		unpost_menu(menu);
		free_menu(menu);
		/* clr_all() */
		if (!idx) {
			for (i = 0; i < idx_old; i++) {
				free_item(items[i]);
				items[i] = NULL;
			}
			wrefresh(w_mid);
			idx_old = idx;
			return NULL;
		}
	}
	/* add a new, idx == idx_old + 1 */
	if (idx > idx_old) {
		/* idx 1~n, decl */
		items[idx - 1] = new_item(res[idx - 1], NULL);
		set_item_userptr(items[idx - 1], delete);
	/* clr_one() */
	} else if (idx == idx_old) {
		/* idx 1~n, decl */
		free_item(items[idx - 1]);
		items[idx - 1] = NULL;
		--idx_old;
		goto update;
	/* clr_any() */
	} else if (idx < idx_old) {
		for (i = 0; i < idx_old; i++) {
			free_item(items[i]);
			items[i] = NULL;
		}
		for (i = 0; i < idx; i++) {
			items[i] = new_item(res[i], NULL);
			set_item_userptr(items[i], delete);
		}
	}

	idx_old = idx;
update:
        menu = new_menu(items);
        menu_opts_off(menu, O_ROWMAJOR | O_SHOWDESC);
        set_menu_win(menu, w_mid);
        set_menu_sub(menu, derwin(w_mid, 0, 0, 1, 25));
        set_menu_format(menu, LINES - 3, 1);
	set_menu_mark(menu, "");
	post_menu(menu);
	wrefresh(w_mid);
	return NULL;
}


/* just for clear all */
static void *restore_table_menu()
{
	char sql[100];
	int qty;
	struct elem *elem;
	while (elem = stack_pop(stk)) {
 		qty = elem->qty;
		sprintf(sql, 
			"update menu set stocks = stocks %c %d, acc = acc %c %d \
			 where id = %d",
			plus, qty, mins, qty, elem->id);
		sqlite3_exec(get_db_main(), sql, 0, 0, 0);
	}
	return NULL;
}

static void *update_table_bill()
{
	if (in_out) {
		price_total = 0 - price_total;
		cost_total = 0 - cost_total;
	}
	char sql[100];
	/* char *err = calloc(1, 100); */
	sprintf(sql, 
		"insert into bill (date, time, sales, cost, profil) \
		 values ('%s', '%s', %.1f, %.1f, %.1f)",
		get_date_time(GET_DATE), get_date_time(GET_TIME),
		price_total, cost_total, price_total - cost_total);
	sqlite3_exec(get_db_main(), sql, 0, 0, 0);
	/* fprintf(stderr, "%s\r\n", err); */
	return NULL;
}

void *sales(MENU *mn)
{
	/* index: 0 - income, 2 - outcome */
	in_out = item_index(current_item(mn));
	if (in_out) {
		plus = '-';
		mins = '+';
	} else {
		plus = '+';
		mins = '-';
	}

	int i;
	WINDOW *w_right = get_win(W_RIGHT);
        fields = (FIELD **)calloc(FIELD_CNT + 1, sizeof (FIELD *));
	/* create & set user ptr */
	for (i = 0; i < FIELD_CNT; ++i) {
		fields[i] = new_field(1, LEN, i * 2, 0, 0, 0);
		set_field_userptr(fields[i], event[i]);
	}
	/* set attr */
	for (i = 0; i < 3; ++i) {
		set_field_back(fields[i], COLOR_PAIR(1));
		set_field_just(fields[i], JUSTIFY_LEFT);
		field_opts_off(fields[i], O_EDIT | O_AUTOSKIP);  	
		/* field_opts_off(fields[i], O_ACTIVE); */
	}
	for (; i < FIELD_CNT - 1; ++i) {
		/* 1 - precision */
		set_field_type(fields[i], TYPE_NUMERIC, 1, 0.0, 999.9);
		set_field_just(fields[i], JUSTIFY_LEFT);
		field_opts_off(fields[i], O_AUTOSKIP);  	
	}
	set_field_type(fields[DECODE], TYPE_INTEGER, 4, 1, 9999);
	set_field_back(fields[SURE], COLOR_PAIR(1));
	set_field_just(fields[SURE], JUSTIFY_LEFT);
	field_opts_off(fields[SURE], O_EDIT | O_AUTOSKIP);  	

        FORM *form = new_form(fields);
	int rows, cols;
	scale_form(form, &rows, &cols);

        set_form_win(form, w_right);
        set_form_sub(form, derwin(w_right, rows, cols, Y, X + 7));
        post_form(form);
        /* wrefresh(w_right); */
	set_field_buffer(fields[CLR_ALL], 0, "取消全部");
	set_field_buffer(fields[CLR_ANY], 0, "取消任意");
	set_field_buffer(fields[CANCEL],  0, "取消上一个");
	set_field_buffer(fields[SURE],    0, "确定");
	set_current_field(form, fields[DECODE]);
	/* DEL wouldn't be converted to REQ_PREV_FIELD */
	form_opts_off(form, O_BS_OVERLOAD);

	mvwprintw(w_right, Y + DECODE   * 2, X, "代码");
	mvwprintw(w_right, Y + DISCOUNT * 2, X, "折扣");
	mvwprintw(w_right, Y + TOTAL    * 2, X, "总价");
	mvwprintw(w_right, Y + CHARGE   * 2, X, "收款");
	pos_form_cursor(form);
        wrefresh(w_right);
	wrefresh(get_win(W_MID));

	WIDGET *widget = widget_init(w_right, form,
				     (FUNCP)unpost_form, 
				     (FUNCP)free_form,
				     (FUNCP)free_field,
			             kb_response, DESC_NO);
	if (!stocks_benchmark) {
		char sql[100];
		sprintf(sql, 
		       "select value from config where key = 'stocks_benchmark'");
		db_select_1_row(get_db_main(), sql, 1,
				 SELECT_TEXT, sql);
		stocks_benchmark = atoi(sql);
	}
	stk = stack_create(ITEM_NUM);
	res = bulk_space(ITEM_NUM, ITEM_SIZE);
	items = calloc(ITEM_NUM + 1, sizeof (ITEM *));

	curs_set(1);
	interact(widget);
	curs_set(0);

	/*-----------------------------------------------------------------------------
	 *  ready to leave the form
	 *  destruct:
	 *  menu/item form/field res stack
	 *-----------------------------------------------------------------------------*/
	free(res);
	stack_free(stk);
	unpost_menu(menu);
	menu = NULL;		/* significant, make the static one back to 0 */
	for (int i = 0; i < idx; ++i) {
		free_item(items[i]);
	}
	free_menu(menu);
	free_widget(widget, (void **)fields, FIELD_CNT);
	/* if invocated clr_any(), widget_bill would be set */
	if (widget_bill) {
		/* don't use free_widget 
		 * because it frees a lot more which have been freed above */
		free(widget_bill);
		widget_bill = NULL;
	}
	wrefresh(w_right);
	return 0;
}

double get_price_total()
{
	return price_total;
}

double get_discount()
{
	return disc;
}

double get_before_dis()
{
	return before_dis;
}

double get_receive_total()
{
	return receive_total;
}

double get_change()
{
	return change;
}

static int any_more(FORM *form)
{
	if (!idx) {
		print_notice(" 已经退完了，不给再退了");
		set_field_buffer(fields[DISCOUNT], 0, "");
		set_field_buffer(fields[TOTAL], 0, "");
		set_field_buffer(fields[CHARGE], 0, "");
		pos_form_cursor(form);
		return 1;
	}
	return 0;
}

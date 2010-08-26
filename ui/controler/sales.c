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

#include	"widget.h"
#include	"event.h"
#include	"glue.h"
#include	"stack.h"

#define		FIELD_CNT	7
#define		LEN		10
#define		Y		15
#define		X		6

enum FIELDS {
	CLR_ALL,
	CANCEL,
	DECODE,
	DISCOUNT,
	TOTAL,
	CHARGE,
	SURE,
};

/* func declaration */
static void *var_reset();
static int validation(FORM *);
static void *clr_all(FORM *);
static void *cancel(FORM *);
static void *decode(FORM *);
static void *discount(FORM *);
static void *total(FORM *);
static void *charge(FORM *);
static void *sure(FORM *);
static void *field_update(FIELD *, float);
static void *menu_update();
static void *update_table_menu();
static void *update_table_bill();


/* local variables */
static FUNCP kb_response[] = {
	form_direct,
	form_enter,
	form_backspace,
	NULL,
	form_input
};
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

static int stocks_benchmark;
static int idx;
static float price_total;
static float cost_total;
static float profil_total;
static struct elem info;


/* local func definition */
static void *var_reset()
{
	/* no stocks_benchmark */
	idx = 0;
	price_total = 0;
	cost_total = 0;
	profil_total = 0;
	info.id = 0;
	*info.name = '\0';
	info.price = 0.0;
	info.cost = 0.0;
	info.stocks = 0;
	info.qty = 0;
	return NULL;
}

static int validation(FORM *form) 
{
	if (E_INVALID_FIELD == form_driver(form, REQ_VALIDATION)) {
		print_notice("输入非法啊大哥，重来");
		form_driver(form, REQ_CLR_FIELD);
		return E_INVALID_FIELD;
	}
	return E_OK;
}

static void *clr_all(FORM *form)
{
	if (!idx) {
		print_notice("已经退完了，不给再退了");
		pos_form_cursor(form);
		return NULL;
	}

	stack_reset(stk);
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
	pos_form_cursor(form);
	var_reset();
	return NULL;
}

static void *cancel(FORM *form)
{
	char sql[100];
	int qty;
	if (!idx) {
		print_notice("已经退完了，不给再退了");
		pos_form_cursor(form);
		return NULL;
	}

	*res[--idx] = '\0';
	struct elem *tmp = stack_pop(stk);
	qty = tmp->qty;
	sprintf(sql, 
		"update menu set stocks = stocks + %d, acc = acc - %d \
		 where id = %d",
		qty, qty, tmp->id);
	sqlite3_exec(get_db_main(), sql, 0, 0, 0);

	price_total -= (tmp->qty * tmp->price);
	cost_total  -= (tmp->qty * tmp->cost);
	/* fields reset */
	field_update(fields[TOTAL], price_total);
	set_field_buffer(fields[DISCOUNT], 0, "");
	set_field_buffer(fields[CHARGE], 0, "");
	/* display reset */
	menu_update();
	pos_form_cursor(form);
	return NULL;
}

static void *decode(FORM *form)
{
	int qty;
	char code[5] = {0};
	int id;
	char sql[100];
	if (E_INVALID_FIELD == validation(form)) {
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
		print_notice("没有代号为%d的菜，请检查输入", info.id);
		form_driver(form, REQ_CLR_FIELD);
		return NULL;
	}
	/* WARNING, maybe not enough */
	if (info.stocks < info.qty) {
		print_notice("强烈警告，%s库存仅%d份，可能不够！\n"
			     "可能是数据未及时更新，所以我不会否决该笔交易", 
			     info.name, info.stocks);	
	}
	sprintf(sql, 
		"update menu set stocks = stocks - %d, acc = acc + %d \
		 where id = %d",
		qty, qty, info.id);
	sqlite3_exec(get_db_main(), sql, 0, 0, 0);

	form_driver(form, REQ_CLR_FIELD);
	price_total += (info.price * info.qty);
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
	stack_push(stk, &info);
	sprintf(res[idx], "%-10s %d份  单价%4.1f  累计%5.1f", 
			   info.name, info.qty, info.price, price_total);
	field_update(fields[TOTAL], price_total);
	menu_update();

	/*-----------------------------------------------------------------------------
	 * name is static, make *name == NUL
	 * see if (!*name)
	 *-----------------------------------------------------------------------------*/
	++idx;
	info.name[0] = '\0';
	return NULL;
}

static void *discount(FORM *form)
{
	if (E_INVALID_FIELD == validation(form)) {
		return NULL;
	}
	const FIELD *field = current_field(form);
	double dis = atof(field_buffer(field, 0)) / 10;
	if (!dis) {
		return NULL;
	}
	field_update(fields[TOTAL], dis * price_total);
	form_driver(form, REQ_DOWN_FIELD);
	form_driver(form, REQ_END_LINE);
	return NULL;
}

static void *total(FORM *form)
{
	if (E_INVALID_FIELD == validation(form)) {
		return NULL;
	}
	const FIELD *field = current_field(form);
	double tmp = atof(field_buffer(field, 0));
	if (!tmp) {
		return NULL;
	}
	price_total = tmp;
	form_driver(form, REQ_DOWN_FIELD);
	form_driver(form, REQ_END_LINE);
	return NULL;
}

static void *charge(FORM *form)
{
	form_driver(form, REQ_DOWN_FIELD);
	form_driver(form, REQ_END_LINE);
	return NULL;
}

static void *sure(FORM *form)
{
	if (stack_empty(stk)) {
		print_notice("还没买菜就急着付钱。。。");
		return NULL;
	}
	/* update_table_menu(); */
	update_table_bill();
	clr_all(form);
	set_current_field(form, fields[DECODE]);
	return NULL;
}

static void *field_update(FIELD *field, float x)
{
	/* update fields-total */
	char tmp[6] = {0};	/* xxx.x + '\0' */
	sprintf(tmp, "%.1f", x);
	set_field_buffer(field, 0, tmp);
	return NULL;
}

static void *menu_update()
{
	/* update menu in the mid window */
	if (menu) {
		unpost_menu(menu);
		free_menu(menu);
	}
	if (!items) {
        	items = calloc(ITEM_NUM + 1, sizeof (ITEM *));
	}
	items[idx] = new_item(res[idx], NULL);
        menu = new_menu(items);
        menu_opts_off(menu, O_SHOWDESC);
	WINDOW *w_mid = get_win(W_MID);
        set_menu_win(menu, w_mid);
        set_menu_sub(menu, derwin(w_mid, 0, 0, 5, 2));
        set_menu_format(menu, 20, 2);
	post_menu(menu);
	wrefresh(w_mid);
	return NULL;
}

void *sales()
{
	int i;
	WINDOW *w_right = get_win(W_RIGHT);

	if (!fields) {
        	fields = (FIELD **)calloc(FIELD_CNT + 1, sizeof (FIELD *));
	}
	for (i = 0; i < 2; ++i) {
		/* 1 fields per 2 lines */
		fields[i] = new_field(1, LEN, i, 0, 0, 0);
		set_field_back(fields[i], COLOR_PAIR(1));
		set_field_just(fields[i], JUSTIFY_LEFT);
		field_opts_off(fields[i], O_EDIT | O_AUTOSKIP);  	
		/* field_opts_off(fields[i], O_ACTIVE);  	 */
	}
	for (; i < 6; ++i) {
		fields[i] = new_field(1, LEN, i, 0, 0, 0);
		/* 1 - precision */
		set_field_type(fields[i], TYPE_NUMERIC, 1, 0.0, 999.9);
		if (i == 2) {
			set_field_type(fields[i], TYPE_INTEGER, 4, 1, 9999);
		}
		set_field_back(fields[i], A_UNDERLINE);
		set_field_just(fields[i], JUSTIFY_LEFT);
		field_opts_off(fields[i], O_AUTOSKIP);  	
	}

	set_field_userptr(fields[CLR_ALL], clr_all);
	set_field_userptr(fields[CANCEL], cancel);
	set_field_userptr(fields[DECODE], decode);
	set_field_userptr(fields[DISCOUNT], discount);
	set_field_userptr(fields[TOTAL], total);
	set_field_userptr(fields[CHARGE], charge);
	set_field_userptr(fields[SURE], sure);
		fields[i] = new_field(1, LEN, i, 0, 0, 0);
		set_field_back(fields[i], COLOR_PAIR(1));
		set_field_just(fields[i], JUSTIFY_LEFT);
		field_opts_off(fields[i], O_AUTOSKIP);  	
		field_opts_off(fields[i], O_EDIT);  	

        FORM *form = new_form(fields);
	int rows, cols;
	scale_form(form, &rows, &cols);

        set_form_win(form, w_right);
        set_form_sub(form, derwin(w_right, rows, cols, Y, X));
        post_form(form);
        /* wrefresh(w_right); */
	set_field_buffer(fields[0], 0, "取消全部");
	set_field_buffer(fields[1], 0, "取消上一个");
	set_field_buffer(fields[6], 0, "确定");
	set_current_field(form, fields[2]);
	/* DEL wouldn't be converted to REQ_PREV_FIELD */
	form_opts_off(form, O_BS_OVERLOAD);

	mvwprintw(w_right, Y + 2, 0, "代码");
	mvwprintw(w_right, Y + 3, 0, "折扣");
	mvwprintw(w_right, Y + 4, 0, "总价");
	mvwprintw(w_right, Y + 5, 0, "收款");
	pos_form_cursor(form);
        wrefresh(w_right);

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

	if (!stk) { 
		stk = stack_create(ITEM_NUM);
		res = bulk_space(ITEM_NUM, ITEM_SIZE);
	}

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
	for (int i = 0; i < idx; ++i) {
		free_item(items[i]);
	}
	free_menu(menu);
	free_widget(widget, (void **)fields, FIELD_CNT);
	wrefresh(w_right);
	return 0;
}

static void *update_table_menu()
{
	char sql[100];
	int qty;
	struct elem *elem;
	while (elem = stack_pop(stk)) {
 		qty = elem->qty;
		sprintf(sql, 
			"update menu set stocks = stocks - %d, acc = acc + %d \
			 where id = %d",
			qty, qty, elem->id);
		sqlite3_exec(get_db_main(), sql, 0, 0, 0);
	}
	return NULL;
}

static void *update_table_bill()
{
	char sql[100];
#if 0
	fprintf(stderr, "%s\r\n", get_date_time(GET_DATE));
	fprintf(stderr, "%s\r\n", get_date_time(GET_TIME));
	fprintf(stderr, "%.1f\r\n", price_total);
	fprintf(stderr, "%.1f\r\n", cost_total);
	__asm__ __volatile__("jmp .");
#endif
	char *err = calloc(1, 100);
	sprintf(sql, 
		"insert into bill (date, time, sales, cost, profil) \
		 values ('%s', '%s', %.1f, %.1f, %.1f)",
		get_date_time(GET_DATE), get_date_time(GET_TIME),
		price_total, cost_total, price_total - cost_total);
	sqlite3_exec(get_db_main(), sql, 0, 0, &err);
	/* fprintf(stderr, "%s\r\n", err); */
	return NULL;
}

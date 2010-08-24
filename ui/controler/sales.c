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

#define		TOTAL		3
#define		DISCOUNT	4
#define		RECEIVE		5


static int validation(FORM *);
static void *decode(FORM *);
static void *makesure();


static FUNCP kb_response[] = {
	form_direct,
	form_enter,
	form_backspace,
	NULL,
	form_input
};

static FIELD **fields;
static MENU *menu;
static ITEM **items;
static char *id;	/* 4 + '\0' */
static char name[50];
static double price;
static double cost;
static int stocks;
static int qty;
static double price_total;
static double cost_total;
static double profil_total;
static char **res;
static stack_t *stk;

static int validation(FORM *form) 
{
	if (E_INVALID_FIELD == form_driver(form, REQ_VALIDATION)) {
		print_notice("输入非法啊大哥，重来");
		form_driver(form, REQ_CLR_FIELD);
		return E_INVALID_FIELD;
	}
	return E_OK;
}

static void *decode(FORM *form)
{
	char sql[100];
	if (E_INVALID_FIELD == validation(form)) {
		return NULL;
	}
	const FIELD *field = current_field(form);
	id = field_buffer(field, 0);
	id[4] = '\0';		/* make sure a following NUL */
	/* 2013 = 201 * 3 */
	qty = id[3] - 0x30;	/* char -> int */
	id[3] = '\0';
	sprintf(sql, 
	       "select name, price, cost, stocks from menu where id = '%s'",
		id);
	db_select_1_row(get_db_main(), sql, 4,
			 SELECT_TEXT, &name,
			 SELECT_DOUBLE, &price,
			 SELECT_DOUBLE, &cost,
			 SELECT_INT, &stocks);
	if (!*name) {
		print_notice("没有代号为%s的菜，请检查输入", id);
		form_driver(form, REQ_CLR_FIELD);
		return NULL;
	}

	form_driver(form, REQ_CLR_FIELD);
	price_total += (price * qty);
	cost_total += (cost * qty);
	static int idx;
	if (!stk) {
		stk = stack_create();
		res = bulk_space(ITEM_NUM, ITEM_SIZE);
	}
	static int acc_num = ITEM_NUM;
	if (idx == acc_num) {
		char **res_bak = res;
		free(res);
		res = bulk_expand(res_bak, acc_num, ITEM_SIZE, ITEM_NUM);
		/* ought to be after bulk_expand */
		acc_num += ITEM_NUM;
		/* meanwhile expand the items */
		ITEM **items_bak = items;
		free(items);
		/* acc_num has been enlarged */
		items = calloc(acc_num + 1, sizeof (ITEM *));
	}

	/* store */
	stack_push(stk, cost);
	sprintf(res[idx], "%-10s %d份  单价%4.1f  合计%5.1f", 
			   name, qty, price, price_total);
	/* update fields-total */
	char tmp[6];	/* xxx.x + '\0' */
	sprintf(tmp, "%.1f", price_total);
	set_field_buffer(fields[TOTAL], 0, tmp);

	/* update mid-menu */
	if (menu) {
		unpost_menu(menu);
		free_menu(menu);
	}
	if (!items) {
        	items = calloc(ITEM_NUM + 1, sizeof (ITEM *));
	}
	items[idx] = new_item(res[idx], NULL);
        menu = new_menu(items);
        menu_opts_off(menu, O_SHOWDESC | O_ROWMAJOR);
	WINDOW *w_mid = get_win(W_MID);
        set_menu_win(menu, w_mid);
        set_menu_sub(menu, derwin(w_mid, 0, 0, 5, 2));
        set_menu_format(menu, 20, 3);
	post_menu(menu);
	wrefresh(w_mid);
	/*-----------------------------------------------------------------------------
	 * name is static, make *name == NUL
	 * see if (!*name)
	 *-----------------------------------------------------------------------------*/
	++idx;
	name[0] = '\0';
	return NULL;
}

static void *makesure()
{
	/* free(res); */
	return NULL;
}

void *sales()
{
	int i;
	WINDOW *win = get_win(W_RIGHT);

	if (!fields) {
        	fields = (FIELD **)calloc(FIELD_CNT + 1, sizeof (FIELD *));
	}
	for (i = 0; i < 2; ++i) {
		/* 1 fields per 2 lines */
		fields[i] = new_field(1, LEN, i, 0, 0, 0);
		set_field_back(fields[i], COLOR_PAIR(1));
		set_field_just(fields[i], JUSTIFY_LEFT);
		field_opts_off(fields[i], O_EDIT | O_AUTOSKIP);  	
		/* field_opts_off(fields[i], O_EDIT);  	 */
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

	set_field_userptr(fields[2], decode);
		fields[i] = new_field(1, LEN, i, 0, 0, 0);
		set_field_back(fields[i], COLOR_PAIR(1));
		set_field_just(fields[i], JUSTIFY_LEFT);
		field_opts_off(fields[i], O_AUTOSKIP);  	
		field_opts_off(fields[i], O_EDIT);  	

        FORM *form = new_form(fields);
	int rows, cols;
	scale_form(form, &rows, &cols);

        set_form_win(form, win);
        set_form_sub(form, derwin(win, rows, cols, Y, X));
        post_form(form);
        /* wrefresh(win); */
	set_field_buffer(fields[0], 0, "取消全部");
	set_field_buffer(fields[1], 0, "取消上一个");
	set_field_buffer(fields[6], 0, "确定");
	set_current_field(form, fields[2]);
	/* DEL wouldn't be converted to REQ_PREV_FIELD */
	form_opts_off(form, O_BS_OVERLOAD);

	mvwprintw(win, Y + 2, 0, "代码");
	mvwprintw(win, Y + 3, 0, "金额");
	mvwprintw(win, Y + 4, 0, "折扣");
	mvwprintw(win, Y + 5, 0, "收款");
	pos_form_cursor(form);
        wrefresh(win);

	WIDGET *widget = widget_init(win, form,
				     (FUNCP)unpost_form, 
				     (FUNCP)free_form,
				     (FUNCP)free_field,
			             kb_response, DESC_NO);

	curs_set(1);
	interact(widget);
	curs_set(0);

	/* free, including form/fields */
	free_widget(widget, (void **)fields, FIELD_CNT);
	return 0;
}

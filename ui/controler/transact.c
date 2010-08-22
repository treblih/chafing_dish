/*
 * =====================================================================================
 *
 *       Filename:  transact.c
 *
 *    Description:  
 *
 *        Created:  19.08.10
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

static void *collect_money();
static void *get_today_bill();


static char *choice[] = {
        "结帐",
        "今日账单查询",
};

static char *choice_desc[] = {
        "哇卡卡卡卡卡卡要收钱了",
        "查询消费明细/退选部分菜种",
};

static FUNCP event[] = {
	collect_money,
	get_today_bill
};

static FUNCP kb_response_transact[] = {
	menu_direct,
	menu_enter,
	NULL
};

static FUNCP kb_response_bills[] = {
	menu_direct,
	NULL,
	NULL
};

static void *collect_money()
{
	return 0;
}

void *transact()
{
	int choice_n = ARRAY_SIZE(choice);
	WINDOW *win = get_win(W_RIGHT);
	ITEM **item = item_initialize(choice, choice_desc, 
			              event, choice_n, FP_ARRAY);
	/* display in a sinlge col */
	MENU *menu = menu_initialize(win, item, 1);	
	WIDGET *widget = widget_init(win, menu, NULL, 
			             kb_response_transact, DESC_NOTICE);

	interact(widget);

	/* free */
	free_widget(widget);
        unpost_menu(menu);
        for (int i = 0; i < choice_n; ++i) {
                free_item(item[i]);
        }
        free_menu(menu);
	return 0;
}

static void *get_today_bill()
{
	char *date = get_date_time(GET_DATE);
	char **bill_list = get_bill_list(date, USER);
	/* get_sql_item_cnt() ought to be invocated after get_bill_list() */
	int choice_n = get_sql_item_cnt();
	WINDOW *win = get_win(W_MID);

	/* no description, no attach funcion */
	ITEM **item = item_initialize(bill_list, NULL, 
				      NULL, choice_n, FP_SINGLE);
	MENU *menu = menu_initialize(win, item, 3);
	WIDGET *widget = widget_init(win, menu, NULL, 
			             kb_response_bills, DESC_NO);

	interact(widget);

	/* free */
	free_widget(widget);
        unpost_menu(menu);
        for (int i = 0; i < choice_n; ++i) {
                free_item(item[i]);
        }
        free_menu(menu);
	free_bill_list(bill_list);
	return 0;
}

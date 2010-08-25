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
	sales,
	get_today_bill
};

static FUNCP kb_response_transact[] = {
	menu_direct,
	menu_enter,
	NULL,
	NULL,
	NULL
};

static FUNCP kb_response_bills[] = {
	menu_direct,
	NULL,
	NULL,
	NULL,
	NULL
};


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

	/*-----------------------------------------------------------------------------
	 *  init variable menu
	 *-----------------------------------------------------------------------------*/
        MENU *menu = new_menu(item);
        menu_opts_off(menu, O_SHOWDESC);
        set_menu_win(menu, win);
        set_menu_sub(menu, derwin(win, 0, 0, 2, 0));
	/* display in a sinlge col */
        set_menu_format(menu, 10, 3);
        post_menu(menu);
        wrefresh(win);

	WIDGET *widget = widget_init(win, menu, 
				     (FUNCP)unpost_menu, 
				     (FUNCP)free_menu,
				     (FUNCP)free_item,
				     kb_response_bills, DESC_NO);
	interact(widget);

	/* free, including menu/items */
	free_widget(widget, (void **)item, choice_n);
	free_bill_list(bill_list);
	return 0;
}

void *transact()
{
	int choice_n = ARRAY_SIZE(choice);
	WINDOW *win = get_win(W_RIGHT);
	ITEM **item = item_initialize(choice, choice_desc, 
			              event, choice_n, FP_ARRAY);
	/*-----------------------------------------------------------------------------
	 *  init variable menu
	 *-----------------------------------------------------------------------------*/
        MENU *menu = new_menu(item);
        menu_opts_off(menu, O_SHOWDESC);
        set_menu_win(menu, win);
        set_menu_sub(menu, derwin(win, 0, 0, 2, 0));
	/* display in a sinlge col */
        set_menu_format(menu, 10, 1);
        post_menu(menu);
        wrefresh(win);

	WIDGET *widget = widget_init(win, menu,
				     (FUNCP)unpost_menu, 
				     (FUNCP)free_menu,
				     (FUNCP)free_item,
			             kb_response_transact, DESC_NOTICE);

	interact(widget);

	/* free, including menu/items */
	free_widget(widget, (void **)item, choice_n);
	return 0;
}

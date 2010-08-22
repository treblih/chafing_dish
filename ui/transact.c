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

#include 	<menu.h>
#include 	<form.h>
#include 	<stdlib.h>
#include	"ui.h"
#include	"glue.h"

static int collect_money();
static int get_today_bill();


static char *choice[] = {
        "结帐",
        "今日账单查询",
        (char *) NULL
};

static char *choice_desc[] = {
        "哇卡卡卡卡卡卡要收钱了",
        "查询消费明细/退选部分菜种",
        (char *) NULL
};

static FUNCP event[] = {
	collect_money,
	get_today_bill
};

void *transact()
{
	WINDOW *win = get_win(W_RIGHT);
	ITEM **item = item_init(choice, choice_desc, 
			        event, ARRAY_SIZE(choice), FP_ARRAY);
	/* display in a sinlge col */
	menu = menu_init(win, item, 1);	
	WIDGET *widget = widget_init(win, kb_response);

	interact(widget);

	/* free */
	free_widget(widget);
        unpost_menu(menu);
        for (int i = 0; i < choice_n; ++i) {
                free_item(menu_items[i]);
        }
        free_menu(menu);
	return 0;
}

static void *collect_money()
{
	return 0;
}
static void *get_today_bill()
{
	char *date = get_date_time(GET_DATE);
	char **bill_list = get_bill_list(date, USER);
	WINDOW *win = get_win(W_RIGHT);
	ITEM **item = item_init(bill_list, NULL, event, 
			        get_sql_item_cnt(), FP_ARRAY);
	MENU *menu = menu_init(win, item, 3);
	WIDGET *widget = widget_init(win, kb_response);

	interact(widget);

	/* free */
	free_widget(widget);
        unpost_menu(menu);
        for (int i = 0; i < choice_n; ++i) {
                free_item(menu_items[i]);
        }
        free_menu(menu);
	free_bill_list(bill_list);
	return 0;
}

static void *up()
{
	WINDOW *w_notice = get_win(W_NOTICE);
	menu_driver(menu, REQ_UP_ITEM);
	werase(w_notice);
	wprintw(w_notice, "%s", item_description(current_item(menu)));
	pos_menu_cursor(menu);
	wrefresh(w_notice);
	return NULL;
}

static void *down()
{
	WINDOW *w_notice = get_win(W_NOTICE);
	menu_driver(menu, REQ_DONW_ITEM);
	werase(w_notice);
	wprintw(w_notice, "%s", item_description(current_item(menu)));
	pos_menu_cursor(menu);
	wrefresh(w_notice);
	return NULL;
}

static void *enter()
{
	FUNCP f = item_userptr(current_item(menu));
	f();
	pos_menu_cursor(menu);
	return NULL;
}

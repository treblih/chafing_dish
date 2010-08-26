/*
 * =====================================================================================
 *
 *       Filename:  get_today_bills.c
 *
 *    Description:  
 *
 *        Created:  26.08.10
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

static FUNCP kb_response[] = {
	menu_direct,
	NULL,
	NULL,
	NULL
};

void *get_today_bills()
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
        menu_opts_off(menu, O_SHOWDESC | O_ROWMAJOR);
        set_menu_win(menu, win);
        set_menu_sub(menu, derwin(win, 0, 0, 2, 2));
        set_menu_format(menu, LINES - 3, 6);
        post_menu(menu);
        wrefresh(win);

	WIDGET *widget = widget_init(win, menu, 
				     (FUNCP)unpost_menu, 
				     (FUNCP)free_menu,
				     (FUNCP)free_item,
				     kb_response, DESC_NO);
	interact(widget);

	/* free, including menu/items */
	free_widget(widget, (void **)item, choice_n);
	free_bill_list(bill_list);
	return 0;
}

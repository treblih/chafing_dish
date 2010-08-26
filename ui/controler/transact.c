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

static void *ret_vgt()
{
	return NULL;
}

static char *choice[] = {
        "结帐",
        "今日账单查询",
	"退菜"
};

static char *choice_desc[] = {
        "哇卡卡卡卡卡卡要收钱了",
        "查询消费明细/退选部分菜种",
	"消费者有时可能需要小退几个菜"
};

static FUNCP event[] = {
	sales,
	get_today_bills,
	sales
};

static FUNCP kb_response_transact[] = {
	menu_direct,
	menu_enter,
	NULL,
	NULL
};


void *transact()
{
	int choice_n = ARRAY_SIZE(choice);
	WINDOW *w_right = get_win(W_RIGHT);
	ITEM **item = item_initialize(choice, choice_desc, 
			              event, choice_n, FP_ARRAY);
	/*-----------------------------------------------------------------------------
	 *  init variable menu
	 *-----------------------------------------------------------------------------*/
        MENU *menu = new_menu(item);
        menu_opts_off(menu, O_SHOWDESC);
        set_menu_win(menu, w_right);
        set_menu_sub(menu, derwin(w_right, 0, 0, 2, 1));
	set_menu_mark(menu, " * ");
        post_menu(menu);
        wrefresh(w_right);

	WIDGET *widget = widget_init(w_right, menu,
				     (FUNCP)unpost_menu, 
				     (FUNCP)free_menu,
				     (FUNCP)free_item,
			             kb_response_transact, DESC_NOTICE);

	interact(widget);

	/* free, including menu/items */
	free_widget(widget, (void **)item, choice_n);
	return 0;
}

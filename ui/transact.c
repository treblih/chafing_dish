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

int bill();
int check();


static char *choices[] = {
        "结帐",
        "今日账单查询",
        (char *) NULL
};

static char *choices_desc[] = {
        "哇卡卡卡卡卡卡要收钱了",
        "查询消费明细/退选部分菜种",
        (char *) NULL
};

static FUNCP func_p[] = {
	bill,
	check
};

int transact()
{
        int c;
        int choices_n = ARRAY_SIZE(choices);
	WINDOW *w_notice = get_w_notice();

	static WINDOW *w_menu;
	static ITEM **menu_items;
	static MENU *menu;

	if (!w_menu) {
		w_menu = newwin(LINES - 3, 15, 0, COLS - 15);
		keypad(w_menu, TRUE);
		menu_items = (ITEM **)malloc(choices_n * sizeof(ITEM *));
		for (int i = 0; i < choices_n; ++i) {
			menu_items[i] = new_item(choices[i], choices_desc[i]);
			set_item_userptr(menu_items[i], func_p[i]);
		}

		menu = new_menu((ITEM **)menu_items);
		menu_opts_off(menu, O_SHOWDESC);
		set_menu_win(menu, w_menu);
		set_menu_sub(menu, derwin(w_menu, 10, 15, 5, 1));
		set_menu_format(menu, 5, 1);
		set_menu_mark(menu, " * ");

		/* Post the menu */
		post_menu(menu);
		wrefresh(w_menu);
	}

        while ((c = wgetch(w_menu)) != '/') {
                switch (c) {
                        case KEY_DOWN:
				menu_driver(menu, REQ_DOWN_ITEM);
				werase(w_notice);
                                wprintw(w_notice, "%s",
					item_description(
						current_item(menu)));
				pos_menu_cursor(menu);
				wrefresh(w_notice);
				break;
                        case KEY_UP:
                                menu_driver(menu, REQ_UP_ITEM);
				werase(w_notice);
                                wprintw(w_notice, "%s",
					item_description(
						current_item(menu)));
				pos_menu_cursor(menu);
				wrefresh(w_notice);
                                break;
			case KEY_ENTER: /* Enter */
			{
				FUNCP f = item_userptr(current_item(menu));
				/* pos_menu_cursor(menu); */
				break;
			}
                }
        }

	return GO_BACK;
}

int bill()
{
	return 0;
}
int check()
{
	int date = get_date_time(GET_DATE);
	char *bill_list = get_today_bill(date, USER);
	return 0;
}

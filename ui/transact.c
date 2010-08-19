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


static char *choices[] = {
        "结帐",
        "今日账单查询",
        (char *) NULL
};

static char *choices_desc[] = {
        "结帐",
        "可退选部分菜种",
        (char *) NULL
};

static FUNCP func_p[] = {
};

int transact()
{
	WINDOW *w_status = get_w_status();
#if 0
	static int init = 0;
	if (init) {
		goto control;
	}
#endif

	WINDOW *w_menu = newwin(LINES - 3, 15, 0, COLS - 15);
        keypad(w_menu, TRUE);
        int choices_n = ARRAY_SIZE(choices);
        ITEM **menu_items = (ITEM **)malloc(choices_n * sizeof(ITEM *));
        for (int i = 0; i < choices_n; ++i) {
                menu_items[i] = new_item(choices[i], choices_desc[i]);
		set_item_userptr(menu_items[i], func_p[i]);
        }

        /* Crate menu */
        MENU *menu = new_menu((ITEM **)menu_items);

        /* Set menu option not to show the description */
        menu_opts_off(menu, O_SHOWDESC);

        /* Create the window to be associated with the menu */

        /* Set main window and sub window */
        set_menu_win(menu, w_menu);
        set_menu_sub(menu, derwin(w_menu, 10, 15, 5, 1));
        set_menu_format(menu, 5, 1);
        set_menu_mark(menu, " * ");

        /* Post the menu */
        post_menu(menu);
        wrefresh(w_menu);

	init = 1;

        int c;
control:
        while ((c = wgetch(w_menu)) != '/') {
                switch (c) {
                        case KEY_DOWN:
				menu_driver(menu, REQ_DOWN_ITEM);
				werase(w_status);
                                wprintw(w_status, "%s",
					item_description(
						current_item(menu)));
				pos_menu_cursor(menu);
				wrefresh(w_status);
				break;
                        case KEY_UP:
                                menu_driver(menu, REQ_UP_ITEM);
				werase(w_status);
                                wprintw(w_status, "%s",
					item_description(
						current_item(menu)));
				pos_menu_cursor(menu);
				wrefresh(w_status);
                                break;
#if 0
			case KEY_ENTER: /* Enter */
			{
				FUNCP f = item_userptr(current_item(menu));
				if (f()) {
					 goto exit;
				}
				pos_menu_cursor(menu);
				break;
			}
#endif
                }
        }

exit:
	return 0;
}

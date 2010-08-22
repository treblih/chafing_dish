/*
 * =====================================================================================
 *
 *       Filename:  menu.c
 *
 *    Description:  
 *
 *        Created:  22.08.10
 *       Revision:  
 *       Compiler:  GCC 4.4
 *
 *         Author:  Yang Zhang, treblih.divad@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#include	"widget.h"

MENU *menu_init(WINDOW *win, ITEM **item, int cols)
{
	WINDOW *w = get_win(win);
        MENU *menu = new_menu(item);
        menu_opts_off(menu, O_SHOWDESC);
        set_menu_win(menu, w_display);
        set_menu_sub(menu, derwin(w_display, 0, 0, 10, 0));
        set_menu_format(menu, 10, cols);
        post_menu(menu);
        wrefresh(w_display);
	return menu;
}

ITEM **item_init(char **name, char **desc, FUNCP fp, int cnt, int is_fp_single)
{
        ITEM **item = (ITEM **)calloc(cnt + 1, sizeof (ITEM *));

	if (desc) {
		for (int i = 0; i < cnt; ++i) {
			item[i] = new_item(name[i], desc[i]);
		}
	} else {
		for (int i = 0; i < cnt; ++i) {
			item[i] = new_item(name[i], NULL);
		}
	}

	if (is_fp_single) {
		for (int i = 0; i < cnt; ++i) {
			set_item_userptr(item[i], fp);
		}
	} else {
		for (int i = 0; i < cnt; ++i) {
			set_item_userptr(item[i], fp[i]);
		}
	}
	return item;
}

void *menu_direct(MENU *menu, int direct, int desc)
{
	menu_driver(menu, KEY_MAX + direct);
	if (desc) {
		WINDOW *w_notice = get_win(W_NOTICE);
		werase(w_notice);
		wprintw(w_notice, "%s", 
			item_description(current_item(menu)));
	}
	pos_menu_cursor(menu);
	wrefresh(w_notice);
	return NULL;
}

void *enter()
{
	FUNCP f = item_userptr(current_item(menu));
	f();
	pos_menu_cursor(menu);
	return NULL;
}

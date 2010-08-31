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

#include	"event.h"

static void *direct(MENU *, int, int);
static void *enter(MENU *);

static FUNCP kb_response[] = {
	direct,
	enter,
	NULL,
	NULL
};

/*-----------------------------------------------------------------------------
 *  abstract item initialization not menu because of it's variability
 *-----------------------------------------------------------------------------*/
ITEM **item_initialize(char **name, char **desc, FUNCP *fp, int cnt, int is_fp_single)
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
			set_item_userptr(item[i], (FUNCP)fp);
		}
	} else {
		for (int i = 0; i < cnt; ++i) {
			set_item_userptr(item[i], fp[i]);
		}
	}
	return item;
}

static void *direct(MENU *menu, int direct, int desc)
{
	
	/*-----------------------------------------------
	 * /usr/include/menu.h
	 *
	 * #define REQ_LEFT_ITEM           (KEY_MAX + 1)
	 * #define REQ_RIGHT_ITEM          (KEY_MAX + 2)
	 * #define REQ_UP_ITEM             (KEY_MAX + 3)
	 * #define REQ_DOWN_ITEM           (KEY_MAX + 4)
	 *-----------------------------------------------*/
	menu_driver(menu, KEY_MAX + direct);
	if (desc) {
		print_notice("%s", 
			item_description(current_item(menu)));
	}
	/* pos_menu_cursor(menu); */
	return NULL;
}

static void *enter(MENU *menu)
{
	FUNCP f = item_userptr(current_item(menu));
	f(menu);
	/* pos_menu_cursor(menu); */
	return NULL;
}

void *menu_create(menu_t *mt)
{
	int choice_n = mt->choice_n;
	WINDOW *win = mt->win;
	ITEM **items = item_initialize(mt->choice, mt->choice_desc, 
			               mt->userptr, choice_n, mt->userptr_arr);
	/* init variable menu */
        MENU *menu = new_menu(items);
        menu_opts_off(menu, mt->opts_off);
        set_menu_win(menu, win);
        set_menu_sub(menu, derwin(win, mt->ys, mt->xs, mt->y, mt->x));
	set_menu_mark(menu, mt->mark);
        post_menu(menu);
        wrefresh(win);

	WIDGET *widget = widget_init(win, menu,
				     (FUNCP)unpost_menu, 
				     (FUNCP)free_menu,
				     (FUNCP)free_item,
			             kb_response, mt->desc_notice);

	interact(widget);

	/* free, including menu/items */
	free_widget(widget, (void **)items, choice_n);
	return NULL;
}

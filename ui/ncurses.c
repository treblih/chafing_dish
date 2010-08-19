#include	<stdlib.h>
#include	<locale.h>
#include	"ui.h"


static char *choices[] = {
        "收银",
        "后台管理",
        "无痛练习",
        "退出",
        (char *) NULL
};

static char *choices_desc[] = {
        "结帐/查询今日记录",
        "数据查询/分析/管理，需要管理员权限",
        "依据现有数据练习软件操作，结果不会更新进数据库",
        "退出",
        (char *) NULL
};

static FUNCP func_p[] = {
	transact,
	manage,
	practice,
	logout
};


int main()
{
        setlocale(LC_ALL, "zh_CN.UTF-8");
        int c;
	ncurses_init();

        WINDOW *w_menu = newwin(MAIN_YS,
			        MAIN_XS,
				MAIN_Y,
				MAIN_X);
	WINDOW *w_notice = get_w_notice();
	WINDOW *w_status = get_w_status();
        keypad(w_menu, TRUE);

        int choices_n = ARRAY_SIZE(choices);
        /* ITEM **menu_items = (ITEM **)malloc(choices_n * sizeof(ITEM *)); */
        ITEM **menu_items = (ITEM **)calloc(10, sizeof(ITEM *));
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
        set_menu_sub(menu, derwin(w_menu, 0, 0, 10, 0));
        set_menu_format(menu, 5, 1);
        set_menu_mark(menu, " * ");

        /* Post the menu */
        post_menu(menu);
        wrefresh(w_menu);

        while (c = wgetch(w_menu)) {
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
			case 10: /* Enter */
			{
				FUNCP f = item_userptr(current_item(menu));
				if (f()) {
					 goto exit;
				}
				pos_menu_cursor(menu);
				break;
			}
                }
        }

exit:
        /* Unpost and free all the memory taken up */
        unpost_menu(menu);
        for (int i = 0; i < choices_n; ++i) {
                free_item(menu_items[i]);
        }
        free_menu(menu);

        /* getch(); */

        endwin();
        return 0;
}

#if 0
int transact()
{
	return 0;
}
#endif
int manage()
{
	return 0;
}
int practice()
{
	return 0;
}
int logout()
{
	return 1;
}

int ncurses_init()
{
        initscr();
        noecho();
        start_color();
        init_pair(1, COLOR_WHITE, COLOR_BLUE);
        curs_set(0);
        refresh();
	return 0;
}

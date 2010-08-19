/*
 * =====================================================================================
 *
 *       Filename:  tmp.c
 *
 *    Description:  
 *
 *        Created:  20.08.10
 *       Revision:  
 *       Compiler:  GCC 4.4
 *
 *         Author:  Yang Zhang, treblih.divad@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

				unpost_menu(menu);
				free_menu(menu);
				menu_items[4] = new_item("wakakk", "kkkkkk");
				set_item_userptr(menu_items[4], func_p[3]);

        			menu = new_menu((ITEM **)menu_items);
        menu_opts_off(menu, O_SHOWDESC);

        /* Create the window to be associated with the menu */

        /* Set main window and sub window */
        set_menu_win(menu, w_menu);
        set_menu_mark(menu, " @ ");
#if 0
        set_menu_format(menu, 5, 1);
        set_menu_sub(menu, derwin(w_menu, 10, 15, 5, 1));
#endif
				post_menu(menu);
				wrefresh(w_menu);

int get_date()
{
	return date;
}
int get_time()
{
	return time;
}
select 

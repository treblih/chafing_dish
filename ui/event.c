/*
 * =====================================================================================
 *
 *       Filename:  interact.c
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

int interact(WIDGET *widget)
{
	FUNCP f;
	WINDOW *win = get_attach_win(widget);
	widget->menu ? (MENU *wid = widget->menu)
		     : (FORM *wid = widget->form);
	int desc = widget->desc;
        while ((c = wgetch(win)) != '/') {
                switch (c) {
		case KEY_LEFT:
			if (f = widget->direct())
				f(wid, D_LEFT, desc);
			break;
		case KEY_RIGHT:
			if (f = widget->direct())
				f(wid, D_RIGHT, desc);
			break;
		case KEY_UP:
			if (f = widget->direct())
				f(wid, D_UP, desc);
			break;
		case KEY_DOWN:
			if (f = widget->direct())
				f(wid, D_DOWN, desc);
			break;
		case 10: /* Enter */
			if (f = widget->enter())
				f(wid);
			break;
		case '-':
			if (f = widget->dash())
				f(wid);
			break;
		default:
			break;
                }
        }
	return 0;
}

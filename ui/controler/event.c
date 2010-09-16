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

#include	"event.h"

int interact(WIDGET *widget)
{
	int c;
	FUNCP f;
	WINDOW *win = widget->win;
	void *wid = widget->wid;
	int desc = widget->desc;

	box(win, ACS_CKBOARD, ACS_CKBOARD);
        wrefresh(win);
        while ((c = wgetch(win)) != '/') {
                switch (c) {
		case KEY_LEFT:
			if (f = widget->direct)
				f(wid, D_LEFT, desc);
			break;
		case KEY_RIGHT:
			if (f = widget->direct)
				f(wid, D_RIGHT, desc);
			break;
		case KEY_UP:
			if (f = widget->direct)
				f(wid, D_UP, desc);
			break;
		case KEY_DOWN:
			if (f = widget->direct)
				f(wid, D_DOWN, desc);
			break;
		case 10: /* Enter */
			if (f = widget->enter)
				/* clear me, waiting for a forward set */
				wborder(win, ' ', ' ', ' ', ' ', 
					     ' ', ' ', ' ', ' ');
				wrefresh(win);

				/* back here */
				f(wid);

				/* back to me, set myself again */
				box(win, ACS_CKBOARD, ACS_CKBOARD);
				wrefresh(win);
			break;
		case '-':  /* 0x2d */
			if (f = widget->backspace)
				f(wid);
			break;
		/* all printable chars */
		case 0x20 ... 0x2c: 
		case 0x2e ... 0x7e:
			if (f = widget->input)
				f(wid, c);
			break;
		default:
			break;
                }
        }
	/* clear me, waiting for a backward set */
	wborder(win, ' ', ' ', ' ', ' ', 
		     ' ', ' ', ' ', ' ');
	wrefresh(win);
	return 0;
}

void *nothing()
{
	return NULL;
}

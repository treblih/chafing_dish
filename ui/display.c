/*
 * =====================================================================================
 *
 *       Filename:  display.c
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

#include	"ui.h"

WINDOW *get_w_display()
{
	static WINDOW *w_display;
	if (!w_display) {
		w_display = newwin(DISPLAY_YS,
				   DISPLAY_XS,
				   DISPLAY_Y,
				   DISPLAY_X);
		keypad(w_display, TRUE);
		wrefresh(w_display);
	}
	return w_display;
}

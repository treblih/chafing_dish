/*
 * =====================================================================================
 *
 *       Filename:  widget.c
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

WIDGET *widget_init(WINDOW *win, 
		    MENU   *menu, 
		    FORM   *form, 
		    FUNCP  *fp,
		    int    desc)
{
	WIDGET *widget = calloc(1, sizeof (WIDGET));
	widget->win = win;
	widget->menu = menu;
	widget->form = form;
	widget->desc = desc;
	FUNCP *fp_addr = &widget->direct;
	for (int i = 0; i < WIDGET_FUNC_CNT; ++i) {
		*fp_addr++ = fp[i];
	}
	return widget;
}

WINDOW *get_attach_win(WIDGET *widget)
{
	return widget->win;
}

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
		    void   *wid, 	/* panel/menu/form */
		    FUNCP   unpost,
		    FUNCP   free_set,
		    FUNCP   free_elem,
		    FUNCP  *fp,
		    int    desc)
{
	WIDGET *widget = calloc(1, sizeof (WIDGET));
	widget->win = win;
	widget->wid = wid;
	widget->unpost = unpost;
	widget->free_set = free_set;
	widget->free_elem = free_elem;
	widget->desc = desc;
	FUNCP *fp_addr = &widget->direct;
	for (int i = 0; i < WIDGET_FUNC_CNT; ++i) {
		*fp_addr++ = fp[i];
	}
	return widget;
}

int free_widget(WIDGET *widget, void **elems, int cnt)
{
	/* release_menu */
	void *wid = widget->wid;
        widget->unpost(wid);
        for (int i = 0; i < cnt; ++i) {
		widget->free_elem(elems[i]);
        }
	widget->free_set(wid);
	free(elems);
	free(widget);
	elems = NULL;
	widget = NULL;
	return 0;
}

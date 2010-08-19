/*
 * =====================================================================================
 *
 *       Filename:  status.c
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

#include	"ui.h"

WINDOW *get_w_status()
{
	static WINDOW *w_status;
	if (!w_status) {
		w_status = newwin(STATUS_YS,
				  STATUS_XS,
				  STATUS_Y,
				  STATUS_X);
		wbkgd(w_status, COLOR_PAIR(1));
		wrefresh(w_status);
	}
	return w_status;
}

/*
 * =====================================================================================
 *
 *       Filename:  notice.c
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

WINDOW *get_w_notice()
{
	static WINDOW *w_notice;
	if (!w_notice) {
		w_notice = newwin(NOTICE_YS,
				  NOTICE_XS,
				  NOTICE_Y,
				  NOTICE_X);
		wrefresh(w_notice);
	}
	return w_notice;
}

/*
 * =====================================================================================
 *
 *       Filename:  win.c
 *
 *    Description:  
 *
 *        Created:  21.08.10
 *       Revision:  
 *       Compiler:  GCC 4.4
 *
 *         Author:  Yang Zhang, treblih.divad@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#include	"widget.h"

static WINDOW *w_arr[] = {
	w_left,
	w_right,
	w_mid,
	w_status,
	w_notice
};

static int init(WINDOW *w, int ys, int xs, int y, int x, int key)
{
	w = newwin(ys, xs, y, x);
        keypad(w_menu, key);
	wrefresh(w_left);
	return 0;
}

int win_init()
{
	init(w_left, LEFT_YS, LEFT_XS, LEFT_Y, LEFT_X, TRUE);
	init(w_right, RIGHT_YS, RIGHT_XS, RIGHT_Y, RIGHT_X, TRUE);
	init(w_mid, MID_YS, MID_XS, MID_Y, MID_X, TRUE);
	init(w_status, STATUS_YS, STATUS_XS, STATUS_Y, STATUS_X, FALSE);
	init(w_notice, NOTICE_YS, NOTICE_XS, NOTICE_Y, NOTICE_X, FALSE);
	return 0;
}

WINDOW *get_win(int i)
{
	return w_arr[i];
}

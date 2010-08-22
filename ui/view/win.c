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

static WINDOW *w_arr[5];

static int init(WINDOW *w, int ys, int xs, int y, int x, int key)
{
	w = newwin(ys, xs, y, x);
        keypad(w, key);
	wrefresh(w);
	return 0;
}

int win_init()
{
	init(w_arr[0], LEFT_YS, LEFT_XS, LEFT_Y, LEFT_X, TRUE);
	init(w_arr[1], RIGHT_YS, RIGHT_XS, RIGHT_Y, RIGHT_X, TRUE);
	init(w_arr[2], MID_YS, MID_XS, MID_Y, MID_X, TRUE);
	init(w_arr[3], STATUS_YS, STATUS_XS, STATUS_Y, STATUS_X, FALSE);
	init(w_arr[4], NOTICE_YS, NOTICE_XS, NOTICE_Y, NOTICE_X, FALSE);
	return 0;
}

WINDOW *get_win(int i)
{
	return w_arr[i];
}

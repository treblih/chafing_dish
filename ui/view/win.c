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

#include "widget.h"
#include "glue.h"

static WINDOW *w_arr[5];

static int init(WINDOW **win, int ys, int xs, int y, int x, int key, int color)
{
	WINDOW *w = *win = newwin(ys, xs, y, x);
        keypad(w, key);
	if (color) {
		wbkgd(w, color);
	}
	wrefresh(w);
	return 0;
}

int win_init()
{
	init(&w_arr[0], LEFT_YS, LEFT_XS, LEFT_Y, LEFT_X, TRUE, 0);
	init(&w_arr[1], RIGHT_YS, RIGHT_XS, RIGHT_Y, RIGHT_X, TRUE, 0);
	init(&w_arr[2], MID_YS, MID_XS, MID_Y, MID_X, TRUE, 0);
	init(&w_arr[3], STATUS_YS, STATUS_XS, STATUS_Y, STATUS_X, FALSE,
	                COLOR_PAIR(1));
	init(&w_arr[4], NOTICE_YS, NOTICE_XS, NOTICE_Y, NOTICE_X, FALSE, 0); 
	return 0;
}

int win_dstr()
{
	for (int i = 0; i < 5; ++i)  {
		delwin(w_arr[i]);
	}
	return 0;
}

WINDOW *get_win(int i)
{
	return w_arr[i];
}

int print_notice(const char *fmt, ...)
{
	va_list str;
	va_start(str, fmt);
	WINDOW *w_notice = get_win(W_NOTICE);
	werase(w_notice);
	int n = vw_printw(w_notice, fmt, str);
	wrefresh(w_notice);
	return n;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  statbar_init
 *  Description:  thread
 *  		  set status bar default display
 *  		  refresh the minute every minute
 * =====================================================================================
 */
void *statbar_init()
{
	char *time;
	char *date = get_date_time(GET_DATE);
	char padding[10] = { 0x20 };
	char ssid[100];
	char ssid_passwd[100];
	db_select_1_row(get_db_main(),
			"select value from config where key = 'ssid'",
			1, SELECT_TEXT, ssid);
	db_select_1_row(get_db_main(),
			"select value from config where key = 'ssid_passwd'",
			1, SELECT_TEXT, ssid_passwd);
	WINDOW *w_status = get_win(W_STATUS);
	wprintw(w_status, "%50s", date);
	/* wprintw(w_status, "     %s", get_date_time(GET_WDAY)); */
	wprintw(w_status, "     SSID：%s", ssid);
	wprintw(w_status, "     PASSWORD：%s", ssid_passwd);
	while (1) {
		time = get_date_time(GET_TIME);
		wprintw(w_status, "\r%35s", time);
		wrefresh(w_status);
		sleep(60);
	}
}

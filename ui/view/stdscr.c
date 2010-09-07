/*
 * =====================================================================================
 *
 *       Filename:  stdscr.c
 *
 *    Description:  
 *
 *        Created:  22.08.10 01:52
 *       Revision:  none
 *       Compiler:  GCC 4.4
 *
 *         Author:  Yang Zhang, treblih.divad@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#include	"event.h"
#include	"glue.h"


int ui_init()
{
	pthread_t *ptp = get_pthread_t(0);
	ncurses_init();
	win_init();

	pthread_create(ptp, NULL, statbar_init, NULL);
	main_menu();
	pthread_cancel(*ptp);

        return 0;
}

int ui_close()
{
	win_dstr();
	endwin();
}

int ncurses_init()
{
        setlocale(LC_ALL, "zh_CN.UTF-8");
        initscr();
        noecho();
	/* cbreak(); */
        start_color();
        init_pair(1, COLOR_WHITE, COLOR_BLUE);
        init_pair(2, COLOR_RED, COLOR_BLACK);
        curs_set(0);

	/*-----------------------------------------------------------------------------
	 *  essential, a bug in ncurses-5.7.4
	 *  need at least one refresh() before any wrefresh()
	 *-----------------------------------------------------------------------------*/
        refresh();
	return 0;
}

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

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

/* need main_menu() */
#include	"event.h"
#include	"glue.h"

static void *statbar_init();
static void *db_flush();

int ui_init()
{
	pthread_t *pt1 = get_pthread_t(0);
	pthread_t *pt2 = get_pthread_t(1);
	ncurses_init();
	win_init();

	pthread_create(pt1, NULL, statbar_init, NULL);
	pthread_create(pt2, NULL, db_flush, NULL);
	/* db_flush(); */
	main_menu();
	pthread_cancel(*pt1);

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

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  statbar_init
 *  Description:  set status bar default display
 *  		  refresh the minute every minute
 * =====================================================================================
 */
static void *statbar_init()
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

static void *db_flush()
{
	char date_last[11] = { 0 };
	char *date_today = get_date_time(GET_DATE);
	db_select_1_row(get_db_main(),
			"select date from bill order by date desc limit 1",
			1, SELECT_TEXT, date_last);
	if (strncmp(date_today, date_last, 10)) {
		char sql[100] = { 0 };
		sprintf(sql, "select sales, cost, profil from bill where date = '%s'", date_last);

		/* 3 cols -- sales/ cost/ profil */
		int col_type[3] = { SELECT_DOUBLE, SELECT_DOUBLE, SELECT_DOUBLE };
		char **res = db_select(get_db_main(), sql, 3, col_type);
		struct daily_total *total = conclusion(res);
		sprintf(sql, "insert into daily (date, count, sales, cost, profil)"
			      "values ('%s', %d, %.1f, %.1f, %.1f)",
			      date_last,
			      total->cnt,
			      total->sales,
			      total->cost,
			      total->profil);
		sqlite3_exec(get_db_main(), sql, 0, 0, 0);
	}
	return NULL;
	/* exit(0); */
}

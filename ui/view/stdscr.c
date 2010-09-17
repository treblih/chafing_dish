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
	/* pthread_t *pt2 = get_pthread_t(1); */
	ncurses_init();
	win_init();

	pthread_create(pt1, NULL, statbar_init, NULL);
	/* pthread_create(pt2, NULL, db_flush, NULL); */
	db_flush();
	main_menu();
	pthread_cancel(*pt1);
	/* pthread_cancel(*pt2); */

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
 *  Description:  thread
 *  		  set status bar default display
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

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  db_flush
 *  Description:  thread
 *  		  everyday first get inthis software, check table bill to make a
 *  		  conclusion for last day/month
 * =====================================================================================
 */
static void *db_flush()
{
	/* pthread, use vars in local */
	char today[11]       = { 0 };	/* 03/01 */
	char bill_last[11]   = { 0 };	/* 02/27 */
	char daily_last[11]  = { 0 };	/* 02/28 */
	char monthly_last[8] = { 0 };	/* 01    */
	char sql[100] 		  = { 0 };
	/* today = get_date_time(GET_DATE); */
	strncpy(today, get_date_time(GET_DATE), 10);

	db_select_1_row(get_db_main(),
			"select date from bill order by date desc limit 1",
			1, SELECT_TEXT, bill_last);
	db_select_1_row(get_db_main(),
			"select date from daily order by date desc limit 1",
			1, SELECT_TEXT, daily_last);
#if 0
	db_select_1_row(get_db_main(),
			"select date from monthly order by date desc limit 1",
			1, SELECT_TEXT, monthly_last);
#endif

	/*-----------------------------------------------------------------------------
	 *  [9] means the last one in '2010/02/28'
	 *  if no cmp with daily_last & bill_last,
	 *  the same daily conclusion will add to table daily alot of times
	 *-----------------------------------------------------------------------------*/
	if ((today[9]      != bill_last[9]) &&
	    (daily_last[9] != bill_last[9])) {
		sprintf(sql, "select sales, cost, profil from bill where date = '%s'", 
			bill_last);

		/* 3 cols -- sales/ cost/ profil */
		int col_type[3] = { SELECT_DOUBLE, SELECT_DOUBLE, SELECT_DOUBLE };
		char **res = db_select(get_db_main(), sql, 3, col_type);
		struct daily_total *total = conclusion(res, 0);
		strncpy(monthly_last, bill_last, 7);
		sprintf(sql, "insert into daily (date, month, count, sales, cost, profil) "
			     "values ('%s', '%s', %d, %.1f, %.1f, %.1f)",
			      /* 2010/02/28 */
			      bill_last,
			      /* 2010/02 */
			      monthly_last,
			      total->cnt,
			      total->sales,
			      total->cost,
			      total->profil);
		sqlite3_exec(get_db_main(), sql, 0, 0, 0);
	}
	if (today[9] == '1' && 
	    daily_last[6] != monthly_last[6]) {
		sprintf(sql, "select sales, cost, profil, count from daily "
			     "where month = '%s'", 
			/* has been 2010/02 */
			bill_last);

		/* 3 cols -- sales/ cost/ profil */
		int col_type[4] = { SELECT_DOUBLE, SELECT_DOUBLE, 
			            SELECT_DOUBLE, SELECT_INT };
		char **res = db_select(get_db_main(), sql, 4, col_type);
		struct daily_total *total = conclusion(res, 0);
		sprintf(sql, "insert into monthly (month, count, sales, cost, profil) "
			     "values ('%s', %d, %.1f, %.1f, %.1f)",
			      bill_last,
			      total->cnt,
			      total->sales,
			      total->cost,
			      total->profil);
		sqlite3_exec(get_db_main(), sql, 0, 0, 0);
	}
	return NULL;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  strcut
 *  Description:  set str[pos] = '\0'
 * =====================================================================================
 */
char *strcut(char *str, int pos)
{
	/* pos: 0 ~ len-1 */
	if (strlen(str) <= pos) {
		return str;
	}
	str[pos] = '\0';
	return str;
}

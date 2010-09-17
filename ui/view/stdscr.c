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
#include "widget.h"
#include "event.h"
#include "glue.h"


int ui_init()
{
	pthread_t *pt1 = get_pthread_t(0);
	pthread_t *pt2 = get_pthread_t(1);
	ncurses_init();
	win_init();

	pthread_create(pt1, NULL, statbar_init, NULL);
	pthread_create(pt2, NULL, db_flush, NULL);
	main_menu();
	pthread_cancel(*pt1);
	pthread_cancel(*pt2);

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

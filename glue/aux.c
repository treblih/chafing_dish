/*
 * =====================================================================================
 *
 *       Filename:  aux.c
 *
 *    Description:  
 *
 *        Created:  20.08.10
 *       Revision:  
 *       Compiler:  GCC 4.4
 *
 *         Author:  Yang Zhang, treblih.divad@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#include	"glue.h"
#include	"db.h"


static int sql_item_cnt;


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  get_date_time
 *  Description:  date changes 1 time per day, so make it long live
 * =====================================================================================
 */
int get_date_time(int req)
{
	static int date;
	if (req == GET_DATE && date) {
		return date;
	}

	/*-----------------------------------------------------------------------------
	 *  need time or haven't init
	 *-----------------------------------------------------------------------------*/
	time_t timep; 
	struct tm *p; 
	time(&timep); 
	p = gmtime(&timep); 
	date = (p->tm_year - 100) * 10000 + (1 + p->tm_mon) * 100 + p->tm_mday;
	if (req == GET_DATE) {
		return date;
	}
	return p->tm_hour * 100 + p->tm_min;
}

char **get_bill_list(int date, int privilege)
{
	char sql[100];
	if (privilege == USER) {
		sprintf(sql, "select time, sales from bill where date = %d", 
			date);
		return db_select(get_db_main(), sql, 2,
				 SELECT_INT,
				 SELECT_DOUBLE);
	}
	sprintf(sql, "select time, sales, cost, profil from bill where date = %d", 
		date);
	return db_select(get_db_main(), sql, 4,
			 SELECT_INT,
			 SELECT_DOUBLE,
			 SELECT_DOUBLE,
			 SELECT_DOUBLE);
}

void free_bill_list(char **res)
{
	free(res);
}

void set_sql_item_cnt(int n)
{
	sql_item_cnt = n;
}

int get_sql_item_cnt()
{
	return sql_item_cnt;
}

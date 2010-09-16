/*
 * =====================================================================================
 *
 *       Filename:  get_today_bills.c
 *
 *    Description:  
 *
 *        Created:  26.08.10
 *       Revision:  
 *       Compiler:  GCC 4.4
 *
 *         Author:  Yang Zhang, treblih.divad@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#include	"event.h"
#include	"glue.h"

void *get_today_bills(MENU *menu)
{
	int i = item_index(current_item(menu));
	char *date = get_date_time(GET_DATE);
	char sql[100];

	/* i == 0 => ADMIN invocation */
	if (i) {
		sprintf(sql, "select time, sales from bill where date = '%s'", date);
		query_menu(2, 6, sql, NULL, 2, SELECT_TEXT, SELECT_DOUBLE);
	} else {
		sprintf(sql, 
		       "select time, sales, cost, profil from bill where date = '%s'", 
		        date);
		query_menu(10, 3, sql, (FUNCP)conclusion, 4, 
			  SELECT_TEXT, 
			  SELECT_DOUBLE,
			  SELECT_DOUBLE,
			  SELECT_DOUBLE);
	}
	return 0;
}

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


static void *conclusion(char **);

void *get_today_bills(MENU *menu)
{
	int i = item_index(current_item(menu));
	char *date = get_date_time(GET_DATE);
	char sql[100];

	/* i == 0 => ADMIN invocation */
	if (i) {
		sprintf(sql, "select time, sales from bill where date = '%s'", date);
		query(2, 6, sql, NULL, 2, SELECT_TEXT, SELECT_DOUBLE);
	} else {
		sprintf(sql, 
		       "select time, sales, cost, profil from bill where date = '%s'", 
		        date);
		query(10, 3, sql, conclusion, 4, 
				  SELECT_TEXT, 
				  SELECT_DOUBLE,
				  SELECT_DOUBLE,
				  SELECT_DOUBLE);
	}
	return 0;
}

static void *conclusion(char **bill_list)
{
	int i = 0;
	int cnt = get_sql_item_cnt();
	double price_total = 0;
	double cost_total = 0;
	double profil_total = 0;
	char res[STR_LEN] = { 0 };
	/* 3 fields - price/cost/profil */
	char **seperate = bulk_space(3, ITEM_SIZE);
	while (*bill_list[i]) {
		strcpy(res, bill_list[i++]);
		/* strtok_r(res, " ", seperate); */
		strdelim(res, ' ', seperate);
		price_total  += atof(seperate[0]);
		cost_total   += atof(seperate[1]);
		profil_total += atof(seperate[2]);
	}
	print_notice("共%d笔生意 总收入%.1f 总成本%.1f 总盈利%.1f", 
		     cnt,
		     price_total,
		     cost_total,
		     profil_total);
	return NULL;
}

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


static int sql_item_cnt;


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  get_date_time
 *  Description:  date changes 1 time per day, so make it long live
 * =====================================================================================
 */
char *get_date_time(int req)
{
	static char date[11];	/* 10 + '\0' */
	static char hm[6];	/* 5 + '\0' */
	if (req == GET_DATE && *date) {		/* date will always be true, *date */
		return date;
	}

	/*-----------------------------------------------------------------------------
	 *  need time or haven't init
	 *-----------------------------------------------------------------------------*/
	time_t timep; 
	struct tm *p; 
	time(&timep); 
	p = gmtime(&timep); 

	if (req == GET_DATE) {
		sprintf(date, "%d/%.2d/%.2d",
			p->tm_year + 1900,
			p->tm_mon + 1,
			p->tm_mday);
		return date;
	}
	sprintf(hm, "%.2d:%.2d",
		p->tm_hour,
		p->tm_min);
	return hm;
}

char **get_bill_list(char *date, int privilege)
{
	/* be careful, on stack */
	char sql[100];
	if (privilege == USER) {
		sprintf(sql, "select time, sales from bill where date = '%s'", 
			date);
		return db_select(get_db_main(), sql, 2,
			      SELECT_TEXT,
			      SELECT_DOUBLE);
	}
	sprintf(sql, "select time, sales, cost, profil from bill where date = '%s'", 
		date);
	return db_select(get_db_main(), sql, 4,
			 SELECT_TEXT,
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

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  get_sql_item_cnt
 *  Description:  called after get_bill_list
 *
 *  get_bill_list -> db_select -> set_sql_item_cnt
 * =====================================================================================
 */
int get_sql_item_cnt()
{
	return sql_item_cnt;
}

char **bulk_space(int cnt, int size)
{
	/* bulk */
	char **res = calloc(cnt, size);
	int str_start = (int)((char *)res + str_offset(cnt));
	int str_addr = str_start;

	/* retail */
	for (int i = 0; i < cnt; ++i) {
		res[i] = (char *)str_addr;
		str_addr += (size - PTR_SIZE);
	}
	return res;
}

char **bulk_expand(char **res, int cnt_orig, int size, int cnt)
{
	int ptrs = str_offset(cnt_orig);
	char **res_large = bulk_space(cnt_orig + cnt, size);
	memcpy(res_large, res, ptrs);
	memcpy((char *)res_large + str_offset(cnt_orig + cnt), 
	       (char *)res       + ptrs, 
	        cnt_orig * (size - PTR_SIZE));
	return res_large;
}

void *spc2zr(char *str)
{
	int ch;
	while ((ch = *str) != '\0') {
		if (ch == 0x20) {
			*str = '\0';
			return NULL;
		} else {
			++str;
		}
	}
	return NULL;
}

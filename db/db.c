/*
 * =====================================================================================
 *
 *       Filename:  db.c
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

#include <pthread.h>
#include "glue.h"

static int init(const char *, sqlite3 **);

static sqlite3 *handle_main;
/* static sqlite3 *handle_daily; */

sqlite3 *get_db_main()
{
	if (!handle_main) {
		init(CHAFING_DB, &handle_main);
	}
	return handle_main;
}

void close_db_main()
{
	sqlite3_close(handle_main);
}

static int init(const char *filename, sqlite3 **handler)
{
	/* if there's no db, create new automatically */
        if (sqlite3_open(filename, handler)) {
                fprintf(stderr, "%s\n", sqlite3_errmsg(*handler));
		return EXIT_FAILURE;
        }
        return EXIT_SUCCESS;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  db_select
 *  Description:  save all results in a format string
 *  		  allocates 4k bytes by default
 *  		  when done, remember to FREE
 * =====================================================================================
 */
char **db_select(sqlite3 *handle, char *sql, int col, int *col_type)
{
	/*-----------------------------------------------------------------------------
	 *  res will contain the format string
	 *  malloc is ok
	 *  64 * (4 + 60)
	 *-----------------------------------------------------------------------------*/
	int type = 0;
	int cnt;
	int total;
	int i = 0;
	int acc_num = ITEM_NUM;
	char **res = bulk_space(ITEM_NUM, ITEM_SIZE);
	char **res_large;
	char *tmp = calloc(STR_LEN * 4, 1);
        sqlite3_stmt *stmt;

        if (sqlite3_prepare_v2(handle, sql, -1, &stmt, 0)) {
                fprintf(stderr, "file: %s  func: %s  line: %d\n%s\n", 
			__FILE__, __func__, __LINE__, sqlite3_errmsg(handle));
		/* longjmp(*get_jmp_buf(), 1); */
        }

	/*-----------------------------------------------------------------------------
	 *  convert sql results to string
	 *  if more than ITEM_NUM, double the array, then loop
	 *-----------------------------------------------------------------------------*/
sql2str:
	for (; sqlite3_step(stmt) != SQLITE_DONE && i < acc_num; ++i) {
        	/* va_start(field, col); */
		total = 0;
                for (int j = 0; j < col; ++j) {
			/* type = va_arg(field, int); */
			type = col_type[j];
			switch (type) {
			case SELECT_INT:
                        	cnt = sprintf(tmp, "%d ", 
				      sqlite3_column_int(stmt, j));
				break;
			case SELECT_DOUBLE:
                        	cnt = sprintf(tmp, "%.1f ", 
				      sqlite3_column_double(stmt, j));
				break;
			case SELECT_TEXT:
                        	cnt = sprintf(tmp, "%s ", 
				      sqlite3_column_text(stmt, j));
				break;
			default:
				fprintf(stderr, "unknown sql field\n");
				break;
			}
			total += cnt;
			tmp += cnt;
                }
		tmp -= total;
		if (total < STR_LEN)	/* 63 + '\0' */ {
			memcpy(res[i], tmp, total);

		/* str is too large */
		} else {
			res_large = calloc(total + 1, 1);		/* '\0' */
			memcpy(res_large, tmp, total);
			res[i] = (char *)res_large;	/* mem leaks */
		}
                /* sqlite3_reset(stmt);   can't add here */
        }

	/* item num is too large, expand */
	if (!(i < acc_num)) {
		char **res_bak = res;
		free(res);
		/* acc_num -- cnt_orig */
		res = bulk_expand(res_bak, acc_num, ITEM_SIZE, ITEM_NUM);
		acc_num += ITEM_NUM;
		goto sql2str;
	}
	set_sql_item_cnt(i);		/* significant */

        if (sqlite3_finalize(stmt)) {
                fprintf(stderr, "file: %s  func: %s  line: %d\n%s\n", 
			__FILE__, __func__, __LINE__, sqlite3_errmsg(handle));
        }
	free(tmp);
        return res;
}

void *db_select_1_row(sqlite3 *handle, char *sql, int col, ...)
{
	int type;
	va_list field;
	va_start(field, col);
        sqlite3_stmt *stmt;

        if (sqlite3_prepare_v2(handle, sql, -1, &stmt, 0)) {
                fprintf(stderr, "file: %s  func: %s  line: %d\n%s\n", 
			__FILE__, __func__, __LINE__, sqlite3_errmsg(handle));
		/* longjmp(*get_jmp_buf(), 1); */
        }
	if (sqlite3_step(stmt) == SQLITE_DONE) {
		goto end;
	}
	for (int i = 0; i < col; ++i) {
		type = va_arg(field, int);
		switch (type) {
		case SELECT_INT:
			*va_arg(field, int *) = 
				sqlite3_column_int(stmt, i);
			break;
		case SELECT_DOUBLE:
			*va_arg(field, double *) = 
				sqlite3_column_double(stmt, i);
			break;
		case SELECT_TEXT:
			/* unique */
			strcpy(va_arg(field, char *), 
			       sqlite3_column_text(stmt, i));
			break;
		default:
			fprintf(stderr, "unknown sql type\n");
			break;
		}
	}

end:
        if (sqlite3_finalize(stmt)) {
                fprintf(stderr, "file: %s  func: %s  line: %d\n%s\n", 
			__FILE__, __func__, __LINE__, sqlite3_errmsg(handle));
        }
	va_end(field);
        return NULL;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  db_flush
 *  Description:  thread
 *  		  everyday first get inthis software, check table bill to make a
 *  		  conclusion for last day/month
 * =====================================================================================
 */
void *db_flush()
{
	/* pthread, use vars in local */
	char today[11]       = { 0 };	/* 03/01 */
	char bill_last[11]   = { 0 };	/* 02/27 */
	char daily_last[11]  = { 0 };	/* 02/28 */
	char monthly_last[8] = { 0 };	/* 01    */
	char sql[100] 	     = { 0 };
	/* today = get_date_time(GET_DATE); */
	strncpy(today, get_date_time(GET_DATE), 10);
	db_select_1_row(get_db_main(),
			"select date from bill order by date desc limit 1",
			1, SELECT_TEXT, bill_last);

	/*-----------------------------------------------------------------------------
	 *  whether insert into daily or not
	 *
	 *  [9] means the last one in '2010/02/28'
	 *  if no cmp with daily_last & bill_last,
	 *  the same daily conclusion will add to table daily alot of times
	 *-----------------------------------------------------------------------------*/
	if (today[9] != bill_last[9]) {
		db_select_1_row(get_db_main(),
				"select date from daily order by date desc limit 1",
				1, SELECT_TEXT, daily_last);

		if (daily_last[9] != bill_last[9]) {
			sprintf(sql, "select sales, cost, profil from bill "
				     "where date = '%s'", bill_last);
			/* 3 cols -- sales/ cost/ profil */
			int col_type[3] = { SELECT_DOUBLE, SELECT_DOUBLE, SELECT_DOUBLE };
			char **res = db_select(get_db_main(), sql, 3, col_type);
			struct daily_total *total = conclusion(res, 0);
			strncpy(monthly_last, bill_last, 7);
			sprintf(sql, "insert into daily "
				     "(date, month, count, sales, cost, profil) "
				     "values "
				     "('%s', '%s', %d, %.1f, %.1f, %.1f)",
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
	}

	/*-----------------------------------------------------------------------------
	 *  whether insert into monthly or ont
	 *-----------------------------------------------------------------------------*/
	if (today[9] == '1') { 
		db_select_1_row(get_db_main(),
				"select month from monthly order by month desc limit 1",
				1, SELECT_TEXT, monthly_last);

		/* make sure daily_last has been inited */
		if ((daily_last[6] != monthly_last[6]) && daily_last[6]) {
			sprintf(sql, "select sales, cost, profil, count from daily "
				     "where month = '%s'", 
				/* has been 2010/02 */
				bill_last);
			/* 3 cols -- sales/ cost/ profil */
			int col_type[4] = { SELECT_DOUBLE, SELECT_DOUBLE, 
					    SELECT_DOUBLE, SELECT_INT };
			char **res = db_select(get_db_main(), sql, 4, col_type);
			struct daily_total *total = conclusion(res, 0);
			sprintf(sql, "insert into monthly "
				     "(month, count, sales, cost, profil) "
				     "values "
				     "('%s', %d, %.1f, %.1f, %.1f)",
				      bill_last,
				      total->cnt,
				      total->sales,
				      total->cost,
				      total->profil);
			sqlite3_exec(get_db_main(), sql, 0, 0, 0);
		}
	}
	return NULL;
	pthread_exit(NULL);
}

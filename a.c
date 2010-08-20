/*
 * =====================================================================================
 *
 *       filename:  a.c
 *
 *    description:  gj
 *
 *        created:  27.05.10
 *       revision:  
 *       compiler:  gcc 4.4
 *
 *         author:  yang zhang, treblih.divad@gmail.com
 *        company:  
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include	<locale.h>
#include	<sqlite3.h>
#include	<time.h>
#include	"ui.h"
#include	"glue.h"
#include	"db.h"

static char **bulk_space(int cnt)
{
	/* bulk */
	char **res = calloc(cnt, ITEM_SIZE);
	int str_start = (int)((char *)res + str_offset(cnt));
	int str_addr = str_start;

	/* retail */
	for (int i = 0; i < cnt; ++i) {
		res[i] = (char *)str_addr;
		str_addr += STR_LEN;
	}
	return res;
}

static char **bulk_expand(char **res, int cnt)
{
	int ptrs = str_offset(cnt);
	char **res_large = bulk_space(cnt + ITEM_NUM);
	memcpy(res_large, res, ptrs);
	memcpy((char *)res_large + str_offset(cnt + ITEM_NUM), 
	       (char *)res       + ptrs, 
	        cnt * STR_LEN);
	return res_large;
}
char **db_select(sqlite3 *handle, char *sql, int col, ...)
{
	/*-----------------------------------------------------------------------------
	 *  res will contain the format string
	 *  malloc is ok
	 *  64 * (4 + 60)
	 *-----------------------------------------------------------------------------*/
	int idx = 0;
	int cnt;
	int total;
	int i = 0;
	int acc_num = ITEM_NUM;
        va_list type;
	char **res = bulk_space(ITEM_NUM);
	char **res_large;
	char *tmp = malloc(STR_LEN * 4);
        sqlite3_stmt *stmt;
	/* char *tmp_cur; */

        if (sqlite3_prepare_v2(handle, sql, -1, &stmt, 0)) {
                fprintf(stderr, "%s\n", sqlite3_errmsg(handle));
        }

	/*-----------------------------------------------------------------------------
	 *  convert sql results to string
	 *  if more than ITEM_NUM, double the array, then loop
	 *-----------------------------------------------------------------------------*/
	fprintf(stderr, "db_select here");
sql2str:
	for (; sqlite3_step(stmt) != SQLITE_DONE && i < acc_num; ++i) {
        	va_start(type, col);
		total = 0;
                for (int j = 0; j < col; ++j) {
			idx = va_arg(type, int);
			switch (idx) {
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
				fprintf(stderr, "unknown sql type\n");
				break;
			}
			fprintf(stderr, "%s\n", tmp);
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
		res = bulk_expand(res_bak, acc_num);
		acc_num += ITEM_NUM;
		goto sql2str;
	}

        if (sqlite3_finalize(stmt)) {
                fprintf(stderr, "%s\n", sqlite3_errmsg(handle));
        }
	free(tmp);
        va_end(type);
        return res;
}
int main()
{
	char *sql = "select time, cost from bill where date = '2010/08/20'";
	sqlite3 *handle;
	sqlite3_stmt *stmt;
	sqlite3_open("chafing.db", &handle);
	char **res = db_select(handle, sql, 2, SELECT_TEXT, SELECT_DOUBLE);
	printf("%s\n", res[0]);
	printf("%s\n", res[1]);
	printf("%s\n", res[2]);
	return 0;
}

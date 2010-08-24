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

#include	"db.h"
#include	"glue.h"

static int init(const char *, sqlite3 **);

static sqlite3 *handle_main;
static sqlite3 *handle_daily;

sqlite3 *get_db_main()
{
	if (!handle_main) {
		init("chafing.db", &handle_main);
	}
	return handle_main;
}

void close_db_main()
{
	sqlite3_close(handle_main);
}

static int init(const char *filename, sqlite3 **addr)
{
        if (sqlite3_open(filename, addr)) {
                fprintf(stderr, "%s\n", sqlite3_errmsg(*addr));
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
char **db_select(sqlite3 *handle, char *sql, int col, ...)
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
	va_list field;
        sqlite3_stmt *stmt;

        if (sqlite3_prepare_v2(handle, sql, -1, &stmt, 0)) {
                fprintf(stderr, "%s\n", sqlite3_errmsg(handle));
        }

	/*-----------------------------------------------------------------------------
	 *  convert sql results to string
	 *  if more than ITEM_NUM, double the array, then loop
	 *-----------------------------------------------------------------------------*/
sql2str:
	for (; sqlite3_step(stmt) != SQLITE_DONE && i < acc_num; ++i) {
        	va_start(field, col);
		total = 0;
                for (int j = 0; j < col; ++j) {
			type = va_arg(field, int);
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
                fprintf(stderr, "%s\n", sqlite3_errmsg(handle));
        }
	free(tmp);
	va_end(field);
        return res;
}

void *db_select_1_row(sqlite3 *handle, char *sql, int col, ...)
{
	int type;
	va_list field;
	va_start(field, col);
        sqlite3_stmt *stmt;

        if (sqlite3_prepare_v2(get_db_main(), sql, -1, &stmt, 0)) {
                fprintf(stderr, "%s\n", sqlite3_errmsg(handle));
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
                fprintf(stderr, "%s\n", sqlite3_errmsg(handle));
        }
	va_end(field);
        return NULL;
}

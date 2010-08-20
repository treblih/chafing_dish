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
static char **bulk_space(int);
static char **bulk_expand(char **, int);

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
	int idx = 0;
	int cnt;
	int total;
	int i = 0;
	int acc_num = ITEM_NUM;
	int offset = 0;
	char *addr;
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
	set_sql_item_cnt(i);		/* significant */

        if (sqlite3_finalize(stmt)) {
                fprintf(stderr, "%s\n", sqlite3_errmsg(handle));
        }
	free(tmp);
        va_end(type);
        return res;
}

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

/*
 * =====================================================================================
 *
 *       Filename:  query.c
 *
 *    Description:  G
 *
 *        Created:  27.08.10
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

static menu_t mt = {
	.ys = 0, .xs = 0, .y = 2,
	.userptr_arr = FP_SINGLE,
	.desc_notice = DESC_NO,
	.mark = "",
	.opts_off = O_SHOWDESC,
	.userptr = (FUNCP *)nothing,
};

void *query(int x, int format_x, char *sql, FUNCP callback, int col, ...)
{
	va_list field;
	va_start(field, col);
	int col_type[col];
	for (int i = 0; i < col; ++i) {
		col_type[i] = va_arg(field, int);
	}
	char **res = db_select(get_db_main(), sql, col, col_type);

	if (callback) {
		callback(res);
	}

	mt.x = x;
	mt.format_x = format_x;
	mt.choice = res;
	mt.choice_n = get_sql_item_cnt();
	mt.win = get_win(W_MID);
	mt.format_y = LINES - 3;

	menu_create(&mt);
	free(res);
	return NULL;
}

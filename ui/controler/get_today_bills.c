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

#include	"widget.h"
#include	"event.h"
#include	"glue.h"


static void *nothing();

static menu_t mt = {
	.ys = 0, .xs = 0, .y = 2, .x = 6,
	.format_x = 6,
	.userptr_arr = FP_SINGLE,
	.desc_notice = DESC_NO,
	.choice_desc = NULL,
	.opts_off = O_SHOWDESC | O_ROWMAJOR,
	/* if all items use 1 userptr */
	.userptr = (FUNCP *)nothing,
};

static void *nothing()
{
	return NULL;
}

void *get_today_bills()
{
	char *date = get_date_time(GET_DATE);
	char **bill_list = get_bill_list(date, USER);
	/* get_sql_item_cnt() ought to be invocated after get_bill_list() */
	mt.choice = bill_list;
	mt.choice_n = get_sql_item_cnt();
	mt.win = get_win(W_MID);
	mt.format_y = LINES - 3;

	menu_create(&mt);
	free_bill_list(bill_list);
	return 0;
}

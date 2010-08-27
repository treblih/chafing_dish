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
	.ys = 0, .xs = 0, .y = 2,
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

void *get_today_bills(MENU *menu)
{
	int i = item_index(current_item(menu));
	char *date = get_date_time(GET_DATE);
	char **bill_list = get_bill_list(date, i);
	/* get_sql_item_cnt() ought to be invocated after get_bill_list() */
	mt.x = i + 4;
	mt.choice = bill_list;
	mt.choice_n = get_sql_item_cnt();
	mt.win = get_win(W_MID);
	mt.format_y = LINES - 3;

	/* i == 0 => ADMIN invocation */
	if (!i) {
		int cnt = get_sql_item_cnt();
		double price_total = 0;
		double cost_total = 0;
		double profil_total = 0;
		char res[STR_LEN] = {0};
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
	}
	menu_create(&mt);
	free_bill_list(bill_list);
	return 0;
}


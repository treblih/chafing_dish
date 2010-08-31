/*
 * =====================================================================================
 *
 *       Filename:  transact.c
 *
 *    Description:  
 *
 *        Created:  19.08.10
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

static char *choice[] = {
        "结帐",
        "今日账单查询",
	"退菜"
};

static char *choice_desc[] = {
        "哇卡卡卡卡卡卡要收钱了",
        "查询消费明细/退选部分菜种",
	"消费者有时可能需要小退几个菜"
};

static FUNCP userptr[] = {
	sales,
	get_today_bills,
	sales
};

static menu_t mt = {
	.ys = 0, .xs = 0, .y = 2, .x = 4,
	.userptr_arr = FP_ARRAY,
	.desc_notice = DESC_NOTICE,
	.choice = choice,
	.choice_desc = choice_desc,
	.mark = " * ",
	.opts_off = O_SHOWDESC,
	.userptr = userptr,
};

void *transact()
{
	mt.choice_n = ARRAY_SIZE(choice);
	mt.win = get_win(W_RIGHT);
	return menu_create(&mt);
}

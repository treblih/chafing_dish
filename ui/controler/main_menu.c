/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  
 *
 *        Created:  22.08.10
 *       Revision:  
 *       Compiler:  GCC 4.4
 *
 *         Author:  Yang Zhang, treblih.divad@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#include	"event.h"


void *practice()
{
	return NULL;
}
void *bug_report()
{
	return NULL;
}

static char *choice[] = {
        "面向客户",
        "面向老板",
        "无痛练习",
	"发送BUG报告",
};

static char *choice_desc[] = {
        "结帐/退菜/查询今日记录",
        "数据查询/分析/管理，需要管理员权限",
        "依据现有数据练习软件操作，结果不会更新进数据库",
	"自动发送记录系统错误文件",
};

static FUNCP userptr[] = {
	transact,
	passwd,
	practice,
	bug_report,
};

static menu_t mt = {
	.ys = 0, .xs = 0, .y = 10, .x = 1,
	.userptr_arr = FP_ARRAY,
	.desc_notice = DESC_NOTICE,
	.choice = choice,
	.choice_desc = choice_desc,
	.mark = " * ",
	.opts_off = O_SHOWDESC,
	.userptr = userptr,
};

void *main_menu()
{
	mt.choice_n = ARRAY_SIZE(choice);
	mt.win = get_win(W_LEFT);
	return menu_interact(&mt);
}

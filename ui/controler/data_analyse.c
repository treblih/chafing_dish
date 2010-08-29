/*
 * =====================================================================================
 *
 *       Filename:  data_analyse.c
 *
 *    Description:  
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

#include	"widget.h"
#include	"event.h"
#include	"glue.h"

static char *choice[] = {
        "今日详细账单",
        "最火爆菜种",
	"最少库存",
	"进入数据库",
	"刷新网络名/密码"
};

static char *choice_desc[] = {
        "今日清单及营收统计",
        "前20",
	"需要尽快补货",
	"启动可视化数据库管理软件，可直接管理数据",
	"当更改过数据库中无线网络和密码设置后，更新底部状态栏显示"
};

static void *popular()
{
	return query(20, 1, 
		  "select id, name, acc from menu order by acc desc limit 20", 
		   NULL, 3, SELECT_INT, SELECT_TEXT, SELECT_INT);
}

static void *lack()
{
	return query(20, 1, 
	          "select id, name, stocks from menu order by stocks limit 20",
	           NULL, 3, SELECT_INT, SELECT_TEXT, SELECT_INT);
}

static void *db_manager()
{
	/* redirect stdout/stderr to /dev/null */
	system("sqliteman ~/chafing/chafing.db >& /dev/null");
	return NULL; 
}

static void *flush()
{
	return statbar_init();
}

static FUNCP userptr[] = {
	get_today_bills,
	popular,
	lack,
	db_manager,
	flush
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

void *data_analyse()
{
	mt.choice_n = ARRAY_SIZE(choice);
	mt.win = get_win(W_RIGHT);
	return menu_create(&mt);
}

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

#include	"event.h"
#include	"glue.h"
#include	<unistd.h>
#include	<sys/types.h>
#include	<sys/wait.h>

static void *date2dbname(char *str);

static char *choice[] = {
        "今日详细账单",
        "最火爆菜种",
	"最少库存",
	"进入数据库",
	/* "刷新网络名/密码", */
	"备份数据库",
};

static char *choice_desc[] = {
        "今日清单及营收统计",
        "前20",
	"需要尽快补货",
	"启动可视化数据库管理软件，可直接管理数据",
	/* "当更改过数据库中无线网络和密码设置后，更新底部状态栏显示", */
	"会自动把数据文件发送至预定邮箱",
};

static void *popular()
{
	return query_menu(20, 1, 
		  "select id, name, acc from menu order by acc desc limit 20", 
		   NULL, 3, SELECT_INT, SELECT_TEXT, SELECT_INT);
}

static void *lack()
{
	return query_menu(20, 1, 
	          "select id, name, stocks from menu order by stocks limit 20",
	           NULL, 3, SELECT_INT, SELECT_TEXT, SELECT_INT);
}

static void *db_manager()
{
	/* redirect stdout/stderr to /dev/null */
	system("sqliteman chafing.db >& /dev/null");
	return NULL; 
}

#if 0
static void *flush()
{
	return statbar_init();
}
#endif

static void *db_bak()
{
	int fd[2];
	pid_t gmimeuuencode;
	pid_t mail;
	char date[14] = { 0 };		/* 2010/08/31.db'\0' */
	char sql[100];
	sprintf(date, "%s", get_date_time(GET_DATE));
	sprintf(sql, "select value from config where key = 'email'");
	db_select_1_row(get_db_main(), sql, 1, SELECT_TEXT, sql);
	if (!*sql) {
		print_notice("数据库中未找到email地址，发送失败，抱歉！");
		return NULL;
	}
	date2dbname(date);
	print_notice(date);

	if (pipe(fd) == -1) {
		perror("pipe error");
		return NULL;
	}
	/* child 1 */
	if ((gmimeuuencode = fork()) == 0) {
		close(fd[0]);
		dup2(fd[1], 1);
		close(fd[1]);
		execlp("gmimeuuencode", "gmimeuuencode", CHAFING_DB, 
			date2dbname(date), NULL);
		perror("gmimeuuencode error");       /* still around? exec failed */
		return NULL;
	}
	/* child 2 */
	if ((mail = fork()) == 0) {
		close(fd[1]);
		dup2(fd[0], 0);
		close(fd[0]);
		/* sql holds the email addr */
		execlp("mail", "mail", "-s", "火锅店数据备份", sql, NULL);
		perror("mail error");
		return NULL;
	}
	return  NULL;
}

static void *date2dbname(char *str)
{
	while (*str) {
		if (*str == '/') {
			*str = '_';
		}
		++str;
	}
	*str++ = '.';
	*str++ = 'd';
	*str++ = 'b';
	*str   = '\0';
	return NULL;
}

static FUNCP userptr[] = {
	get_today_bills,
	popular,
	lack,
	db_manager,
	/* flush, */
	db_bak
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
	return menu_interact(&mt);
}

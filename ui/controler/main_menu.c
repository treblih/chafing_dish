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

#include	"widget.h"
#include	"event.h"


void *manage()
{
	return NULL;
}
void *practice()
{
	return NULL;
}
void *bug_report()
{
	return NULL;
}
void *logout()
{
	return (void *)1;
}

static char *choice[] = {
        "收银",
        "吴哥管理",
        "无痛练习",
	"发送BUG报告",
        "退出"
};

static char *choice_desc[] = {
        "结帐/查询今日记录",
        "数据查询/分析/管理，需要管理员权限",
        "依据现有数据练习软件操作，结果不会更新进数据库",
	"自动发送记录系统错误文件",
        "退出"
};

static FUNCP event[] = {
	transact,
	manage,
	practice,
	bug_report,
	logout
};

static FUNCP kb_response[] = {
	menu_direct,
	menu_enter,
	NULL,		/* dash */
	NULL		/* input */
};

int main_menu()
{
	int choice_n = ARRAY_SIZE(choice);
	WINDOW *win = get_win(W_LEFT);
	ITEM **item = item_initialize(choice, choice_desc, 
			        event, choice_n, FP_ARRAY);

	/*-----------------------------------------------------------------------------
	 *  init variable menu
	 *-----------------------------------------------------------------------------*/
        MENU *menu = new_menu(item);
        menu_opts_off(menu, O_SHOWDESC);
        set_menu_win(menu, win);
        set_menu_sub(menu, derwin(win, 0, 0, 10, 0));
	/* display in a sinlge col */
        set_menu_format(menu, 10, 1);
        post_menu(menu);
        wrefresh(win);

	WIDGET *widget = widget_init(win, menu, 
				     (FUNCP)unpost_menu, 
				     (FUNCP)free_menu,
				     (FUNCP)free_item,
			             kb_response, DESC_NOTICE);

	interact(widget);

	/* free, including menu/items */
	release_widget(widget, (void **)item, choice_n);
	return 0;
}

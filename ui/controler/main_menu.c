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

static FUNCP event[] = {
	transact,
	manage,
	practice,
	bug_report,
};

static FUNCP kb_response[] = {
	menu_direct,
	menu_enter,
	NULL,
	NULL		/* input */
};

int main_menu()
{
	int choice_n = ARRAY_SIZE(choice);
	WINDOW *w_left = get_win(W_LEFT);
	ITEM **item = item_initialize(choice, choice_desc, 
			        event, choice_n, FP_ARRAY);

	/*-----------------------------------------------------------------------------
	 *  init variable menu
	 *-----------------------------------------------------------------------------*/
        MENU *menu = new_menu(item);
        menu_opts_off(menu, O_SHOWDESC);
        set_menu_win(menu, w_left);
        set_menu_sub(menu, derwin(w_left, 0, 0, 10, 1));
	set_menu_mark(menu, " * ");
        post_menu(menu);

	WIDGET *widget = widget_init(w_left, menu, 
				     (FUNCP)unpost_menu, 
				     (FUNCP)free_menu,
				     (FUNCP)free_item,
			             kb_response, DESC_NOTICE);
	interact(widget);

	/* free, including menu/items */
	free_widget(widget, (void **)item, choice_n);
	return 0;
}

/*
 * =====================================================================================
 *
 *       Filename:  ui.h
 *
 *    Description:  
 *
 *        Created:  19.08.10 19:26
 *       Revision:  none
 *       Compiler:  GCC 4.4
 *
 *         Author:  Yang Zhang, treblih.divad@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */
#ifndef         UI_H
#define         UI_H

#include	<menu.h>
#include	<form.h>

#define		GO_BACK		0




extern	int transact();
extern	int manage();
extern	int practice();
extern	int bug_report();
extern	int logout();

extern	int ui_init();
extern	int ncurses_init();
extern	WINDOW *get_w_status();
extern	WINDOW *get_w_notice();
extern	WINDOW *get_w_display();
extern	MENU *menu_generator(char **, int, int);

extern	void bill_detail();

#endif

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

#define 	ARRAY_SIZE(a)	(sizeof (a) / sizeof (a[0]))
#define		GO_BACK		0

/*-----------------------------------------------------------------------------
 *  all windows coordinates
 *-----------------------------------------------------------------------------*/
#define		MAIN_YS		LINES - 3
#define		MAIN_XS		15
#define		MAIN_Y		0
#define		MAIN_X		0

#define		NOTICE_YS	2
#define		NOTICE_XS	COLS
#define		NOTICE_Y	LINES - 2
#define		NOTICE_X	0

#define		STATUS_YS	1
#define		STATUS_XS	COLS
#define		STATUS_Y	LINES - 3
#define		STATUS_X	0

#define		TRANSACT_YS	LINES - 3
#define		TRANSACT_XS	15
#define		TRANSACT_Y	0
#define		TRANSACT_X	COLS - 15


typedef int (*FUNCP)();

extern	int transact();
extern	int manage();
extern	int practice();
extern	int logout();

extern	int ui_init();
extern	int ncurses_init();
extern	WINDOW *get_w_status();
extern	WINDOW *get_w_notice();

#endif

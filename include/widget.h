/*
 * =====================================================================================
 *
 *       Filename:  widget.h
 *
 *    Description:  
 *
 *        Created:  22.08.10 01:52
 *       Revision:  none
 *       Compiler:  GCC 4.4
 *
 *         Author:  Yang Zhang, treblih.divad@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef		WIDGET_H
#define		WIDGET_H

#include	<stdlib.h>
#include	<menu.h>
#include	<form.h>

typedef void *(*FUNCP)();

typedef struct WIDGET {
	WINDOW *win;
	MENU *menu;
	FORM *form;
	FUNCP direct;
	FUNCP enter;
	FUNCP dash;
	int desc;		/* whether item's desc will be shown */
} WIDGET;
#define		WIDGET_FUNC_CNT		3


enum WIN_LAYOUT {
	W_LEFT,
	W_RIGHT,
	W_MID,
	W_STATUS,
	W_NOTICE
};

enum DIRECTION {
	D_LEFT = 1,
	D_RIGHT,
	D_UP,
	D_DOWN
};


/*-----------------------------------------------------------------------------
 *  all windows coordinates
 *-----------------------------------------------------------------------------*/
#define		LEFT_YS		LINES - 3
#define		LEFT_XS		15
#define		LEFT_Y		0
#define		LEFT_X		0

#define		RIGHT_YS	LINES - 3
#define		RIGHT_XS	LEFT_XS
#define		RIGHT_Y	0
#define		RIGHT_X		COLS - 16

#define		MID_YS		LINES - 3
#define		MID_XS		COLS - LEFT_XS - RIGHT_XS - 4
#define		MID_Y		0
#define		MID_X		LEFT_XS + 2

#define		NOTICE_YS	2
#define		NOTICE_XS	COLS
#define		NOTICE_Y	LINES - 2
#define		NOTICE_X	0

#define		STATUS_YS	1
#define		STATUS_XS	COLS
#define		STATUS_Y	LINES - 3
#define		STATUS_X	0

#define		DESC_NO		0
#define		DESC_NOTICE	1

	/* for item_init() */
#define		FP_ARRAY	0
#define		FP_SINGLE	1

#define 	ARRAY_SIZE(a)	(sizeof (a) / sizeof (a[0]))

#define		ui_close()	endwin()
#define		free_widget(a)	free(a)

/* menu.c */
extern MENU *menu_initialize(WINDOW *, ITEM **, int);
extern ITEM **item_initialize(char **, char **, FUNCP *, int, int);
extern void *menu_direct(MENU *, int, int);
extern void *menu_enter(MENU *);

/* stdscr.c */
extern int ui_init(void);
extern int ncurses_init(void);

/* widget.c */
extern WIDGET *widget_init(WINDOW *, MENU *, FORM *, FUNCP *, int);
extern WINDOW *get_attach_win(WIDGET *);

/* win.c */
extern int win_init(void);
extern WINDOW *get_win(int);

#endif

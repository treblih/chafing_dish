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

#include	<locale.h>
#include	<stdlib.h>
#include	<pthread.h>
#include	<unistd.h>
#include	<menu.h>
#include	<form.h>

typedef void *(*FUNCP)();

typedef struct WIDGET {
	WINDOW *win;
	void *wid;		/* panel/menu/form */
	/* sys func */
	FUNCP unpost;
	FUNCP free_set;
	FUNCP free_elem;
	/* user func */
	FUNCP direct;
	FUNCP enter;
	FUNCP backspace;
	FUNCP input;
	int desc;		/* whether item's desc will be shown */
} WIDGET;
#define		WIDGET_FUNC_CNT		4

typedef struct menu_t {
	int ys, xs, y, x;
	int format_y, format_x;
	int userptr_arr;
	int choice_n;
	int desc_notice;
	char **choice;
	char **choice_desc;
	const char *mark;
	Menu_Options opts_off;
	FUNCP *userptr;
	WINDOW *win;
} menu_t;


enum {
	W_LEFT,
	W_RIGHT,
	W_MID,
	W_STATUS,
	W_NOTICE
};

enum {
	D_LEFT = 1,
	D_RIGHT,
	D_UP,
	D_DOWN
};


/*-----------------------------------------------------------------------------
 *  all windows coordinates
 *-----------------------------------------------------------------------------*/
#define		LEFT_YS		LINES - 3
#define		LEFT_XS		18
#define		LEFT_Y		0
#define		LEFT_X		0

#define		RIGHT_YS	LEFT_YS
#define		RIGHT_XS	25
#define		RIGHT_Y		0
#define		RIGHT_X		COLS - RIGHT_XS

#define		MID_YS		LEFT_YS
#define		MID_XS		COLS - LEFT_XS - RIGHT_XS - 2
#define		MID_Y		0
#define		MID_X		LEFT_XS + 1

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


/* menu.c */
extern ITEM **item_initialize(char **, char **, FUNCP *, int, int);
extern void *menu_interact(menu_t *);

/* form.c */
extern void *form_direct(FORM *, int, int);
extern void *form_enter(FORM *);
extern void *form_input(FORM *, int);
extern void *form_backspace(FORM *);
extern int form_validation(FORM *);

/* stdscr.c */
extern int ui_init(void);
extern int ui_close(void);
extern int ncurses_init(void);

/* widget.c */
extern WIDGET *widget_init(WINDOW *, void *, FUNCP, FUNCP, FUNCP, 
		           FUNCP *, int);
extern int free_widget(WIDGET *, void **, int);

/* win.c */
extern int win_init(void);
extern int win_dstr(void);
extern WINDOW *get_win(int);
extern int print_notice(const char *, ...);
extern void *statbar_init();

#endif

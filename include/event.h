/*
 * =====================================================================================
 *
 *       Filename:  event.h
 *
 *    Description:  
 *
 *        Created:  22.08.10 02:32
 *       Revision:  none
 *       Compiler:  GCC 4.4
 *
 *         Author:  Yang Zhang, treblih.divad@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef		EVENT_H
#define		EVENT_H

#define		LEN		10
#define		Y		25
#define		X		4

/* event.c */
extern int interact(WIDGET *);
extern void *nothing();

/* main_menu.c */
extern void *practice(void);
extern void *bug_report(void);
extern void *main_menu(void);

/* transact.c */
extern void *transact();

/* get_today_bills.c */
extern void *get_today_bills(MENU *);

/* sales.c */
extern void *sales();

/* passwd.c */
extern void *passwd();

/* data_analyse.c */
extern void *data_analyse();

/* query.c */
extern void *query(int, int, char *, FUNCP, int, ...);

#endif

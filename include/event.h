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

#include	"widget.h"

#define		LEN		10
#define		Y		18
#define		X		4

/* event.c */
extern int interact(WIDGET *);
extern void *nothing();

/* main_menu.c */
extern void *practice(void);
extern void *bug_report(void);

/* menu.c */
extern void *main_menu(void);
extern FUNCP *get_kb_response_menu();

/* transact.c */
extern void *transact();

/* get_today_bills.c */
extern void *get_today_bills(MENU *);

/* sales.c */
extern void *sales();
extern double get_price_total();
extern double get_discount();
extern double get_before_dis();
extern double get_receive_total();
extern double get_change();

/* passwd.c */
extern void *passwd();

/* data_analyse.c */
extern void *data_analyse();

/* query_menu.c */
extern void *query_menu(int, int, char *, FUNCP, int, ...);

#endif

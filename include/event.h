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

/* event.c */
extern int interact(WIDGET *);

/* main_menu.c */
extern void *manage(void);
extern void *practice(void);
extern void *bug_report(void);
extern int main_menu(void);

/* transact.c */
extern void *transact();

/* get_today_bills.c */
extern void *get_today_bills();

/* sales.c */
void *sales();

#endif

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

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))

typedef int (*FUNCP)();

extern	int transact();
extern	int manage();
extern	int practice();
extern	int logout();
extern	WINDOW *get_w_status();

#endif

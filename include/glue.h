/*
 * =====================================================================================
 *
 *       Filename:  glue.c
 *
 *    Description:  
 *
 *        Created:  20.08.10
 *       Revision:  
 *       Compiler:  GCC 4.4
 *
 *         Author:  Yang Zhang, treblih.divad@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef         GLUE_H
#define         GLUE_H

#include 	<time.h>

	/* for get_date_time */
#define		GET_DATE	0
#define		GET_TIME	1
extern 	int get_date_time(int);

	/* for get_bill_list */
#define		ADMIN		0
#define		USER		1
extern	char **get_bill_list(int, int);

extern	void free_bill_list(char **);
extern	void set_sql_item_cnt(int);
extern	int  get_sql_item_cnt();

#endif

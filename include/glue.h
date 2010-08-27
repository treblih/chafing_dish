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
#include 	"db.h"

#define		PTR_SIZE	sizeof (char *)
#define		STR_LEN		64
#define		ITEM_NUM	32
#define		ITEM_SIZE	(PTR_SIZE + STR_LEN)
#define		ITEM_BYTE	(ITEM_SIZE * ITEM_NUM)

	/* for get_date_time */
#define		GET_DATE	0
#define		GET_TIME	1
extern 	char *get_date_time(int);

	/* for get_bill_list */
#define		ADMIN		0
#define		USER		1
extern	char **get_bill_list(char *, int);

extern	void free_bill_list(char **);
extern	void set_sql_item_cnt(int);
extern	int get_sql_item_cnt();
extern	char **bulk_space(int, int);
extern	char **bulk_expand(char **, int, int, int);
extern	void *spc2zr(char *);

#endif

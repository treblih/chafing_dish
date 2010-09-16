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

#include 	<setjmp.h>
#include 	<time.h>
#include 	"db.h"

struct daily_total {
	int cnt;
	double sales;
	double cost;
	double profil;
};

#define		PTR_SIZE	sizeof (char *)
#define		STR_LEN		64
#define		ITEM_NUM	32
#define		ITEM_SIZE	(PTR_SIZE + STR_LEN)
#define		ITEM_BYTE	(ITEM_SIZE * ITEM_NUM)

	/* for get_date_time */
#define		GET_DATE	0
#define		GET_TIME	1
#define		GET_WDAY	2
extern 	char *get_date_time(int);

	/* for get_bill_list */
#define		ADMIN		0
#define		USER		1

extern	jmp_buf *get_jmp_buf();
extern	void set_sql_item_cnt(int);
extern	int get_sql_item_cnt();
extern	char **bulk_space(int, int);
extern	char **bulk_expand(char **, int, int, int);
extern	void *spc2zr(char *);
extern  void *strdelim(char *, int, char **);
extern	void *list2file(char **);

#define		PTHREAD_NUM	2
extern	pthread_t *get_pthread_t(int);

extern struct daily_total *conclusion(char **);
extern char * strcut(char *, int);

#endif

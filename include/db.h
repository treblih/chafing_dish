/*
 * =====================================================================================
 *
 *       Filename:  db.h
 *
 *    Description:  
 *
 *        Created:  20.08.10 04:14
 *       Revision:  none
 *       Compiler:  GCC 4.4
 *
 *         Author:  Yang Zhang, treblih.divad@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef		DB_H
#define		DB_H

#include	<stdio.h>
#include	<string.h>
#include	<stdlib.h>
#include	<stdarg.h>
#include	<sqlite3.h>

#define		CHAFING_DB	"chafing.db"

	/* for db_select */
enum {
	SELECT_INT,
	SELECT_DOUBLE,
	SELECT_TEXT	
};
extern	char **db_select(sqlite3 *, char *, int, int *);
extern  void *db_select_1_row(sqlite3 *, char *, int, ...);

#define		str_offset(n)	(n) * PTR_SIZE
extern	int db_init(const char *, sqlite3 **);
extern	sqlite3 *get_db_main();
extern	void close_db_main();
#define		db_init()	get_db_main()
extern	void *db_flush();

#endif

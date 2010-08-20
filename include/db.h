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

#define		PTR_SIZE	sizeof (char *)
#define		STR_LEN		64
#define		ITEM_NUM	32
#define		ITEM_SIZE	PTR_SIZE + STR_LEN
#define		ITEM_BYTE	ITEM_SIZE * ITEM_NUM

	/* for db_select */
#define		SELECT_INT	0
#define		SELECT_DOUBLE	1
#define		SELECT_TEXT	2
extern		char **db_select(sqlite3 *, char *, int, ...);

#define		str_offset(n)	(n) * PTR_SIZE
extern		int db_init(const char *, sqlite3 **);

#endif

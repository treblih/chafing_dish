/*
 * =====================================================================================
 *
 *       Filename:  main.c
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

#include	"glue.h"
#include	"ui.h"

int main()
{
	/* get_date_time(); */
	sqlite3 *handle;
	db_init("chafing.db", &handle);
	ui_init();


	sqlite3_close(handle);
	return EXIT_SUCCESS;
}

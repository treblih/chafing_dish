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
#include	"db.h"

int main()
{
	/* get_date_time(); */
	db_init();
	ui_init();

	close_db_main();
	return EXIT_SUCCESS;
}

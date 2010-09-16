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

#include "widget.h"
#include "glue.h"

int main()
{
	db_init();
	if (!setjmp(*get_jmp_buf())) {
		ui_init();
	}
	ui_close();
	close_db_main();
	return EXIT_SUCCESS;
}

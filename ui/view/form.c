/*
 * =====================================================================================
 *
 *       Filename:  form.c
 *
 *    Description:  
 *
 *        Created:  22.08.10
 *       Revision:  
 *       Compiler:  GCC 4.4
 *
 *         Author:  Yang Zhang, treblih.divad@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#include	"widget.h"

void *form_direct(FORM *form, int direct, int desc)
{
	
	/*-----------------------------------------------
	 * /usr/include/form.h
	 *
	 * #define REQ_LEFT_FIELD           (KEY_MAX + 1)
	 * #define REQ_RIGHT_FIELD          (KEY_MAX + 2)
	 * #define REQ_UP_FIELD             (KEY_MAX + 3)
	 * #define REQ_DOWN_FIELD           (KEY_MAX + 4)
	 *-----------------------------------------------*/
	form_driver(form, KEY_MAX + 12 + direct);
	form_driver(form, REQ_END_LINE);
	return NULL;
}

void *form_enter(FORM *form)
{
	FUNCP f = field_userptr(current_field(form));
	f(form);
	return NULL;
}

void *form_input(FORM *form, int ch)
{
	form_driver(form, ch);
	return NULL;
}

void *form_backspace(FORM *form)
{
	form_driver(form, REQ_DEL_PREV);
	return NULL;
}

int form_validation(FORM *form) 
{
	if (E_INVALID_FIELD == form_driver(form, REQ_VALIDATION)) {
		print_notice("输入非法啊大哥，重来");
		form_driver(form, REQ_CLR_FIELD);
		return E_INVALID_FIELD;
	}
	return E_OK;
}

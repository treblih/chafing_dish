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

FORM *form_initialize(WINDOW *win, FIELD **field, int cols)
{
        FORM *form = new_form(field);
        form_opts_off(form, O_SHOWDESC);
        set_form_win(form, win);
        set_form_sub(form, derwin(win, 0, 0, 10, 0));
        set_form_format(form, 10, cols);
        post_form(form);
        wrefresh(win);
	return form;
}

FIELD **field_initialize(char **name, char **desc, FUNCP *fp, int cnt, int is_fp_single)
{
        FIELD **field = (FIELD **)calloc(cnt + 1, sizeof (FIELD *));

	field[1] = new_field(1, 10, 0, 0, 0, 0);
	field_opts_off(field[0], O_AUTOSKIP);  	
	if (desc) {
		for (int i = 0; i < cnt; ++i) {
			field[i] = new_field(name[i], desc[i]);
		}
	} else {
		for (int i = 0; i < cnt; ++i) {
			field[i] = new_field(name[i], NULL);
		}
	}

	if (is_fp_single) {
		for (int i = 0; i < cnt; ++i) {
			set_field_userptr(field[i], (FUNCP)fp);
		}
	} else {
		for (int i = 0; i < cnt; ++i) {
			set_field_userptr(field[i], fp[i]);
		}
	}
	return field;
}

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
	form_driver(form, KEY_MAX + direct);
	if (desc) {
		WINDOW *w_notice = get_win(W_NOTICE);
		werase(w_notice);
		wprintw(w_notice, "%s", 
			field_description(current_field(form)));
		wrefresh(w_notice);
	}
	pos_form_cursor(form);
	return NULL;
}

void *form_enter(FORM *form)
{
	FUNCP f = field_userptr(current_field(form));
	f();
	pos_form_cursor(form);
	return NULL;
}

void *form_input(FORM *form, int ch)
{
	return NULL;
}

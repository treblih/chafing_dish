/*
 * =====================================================================================
 *
 *       Filename:  manage.c
 *
 *    Description:  
 *
 *        Created:  27.08.10
 *       Revision:  
 *       Compiler:  GCC 4.4
 *
 *         Author:  Yang Zhang, treblih.divad@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#include	"widget.h"
#include	"event.h"
#include	"glue.h"

#define		LEN		10
#define		Y		25
#define		X		4

static void *passwd(FORM *form);


static FUNCP kb_response[] = {
	form_direct,
	form_enter,
	form_backspace,
	form_input
};

void *manage()
{
	WINDOW *w_right = get_win(W_RIGHT);
	/* 2 + 1 */
        FIELD **fields = calloc(3, sizeof (FIELD *));
	/* create & set user ptr */
	fields[0] = new_field(1, LEN, 0, 0, 0, 0);
	set_field_userptr(fields[1], passwd);
	set_field_back(fields[0], COLOR_PAIR(1));
	set_field_just(fields[0], JUSTIFY_CENTER);
	field_opts_off(fields[0], O_AUTOSKIP | O_ACTIVE);  	

	/* least validation width */
	fields[1] = new_field(1, LEN, 2, 0, 0, 0);
	set_field_type(fields[1], TYPE_ALNUM, 1);
	set_field_just(fields[1], JUSTIFY_CENTER);
	field_opts_off(fields[1], O_AUTOSKIP | O_PUBLIC);  	

        FORM *form = new_form(fields);
	int rows, cols;
	scale_form(form, &rows, &cols);

        set_form_win(form, w_right);
        set_form_sub(form, derwin(w_right, rows, cols, Y, X + 7));
        post_form(form);
	set_field_buffer(fields[0], 0, "输入密码验证");
	set_current_field(form, fields[1]);
	/* DEL wouldn't be converted to REQ_PREV_FIELD */
	form_opts_off(form, O_BS_OVERLOAD);

	pos_form_cursor(form);
        wrefresh(w_right);

	WIDGET *widget = widget_init(w_right, form,
				     (FUNCP)unpost_form, 
				     (FUNCP)free_form,
				     (FUNCP)free_field,
			             kb_response, DESC_NO);
	curs_set(1);
	interact(widget);
	curs_set(0);

	return NULL;
}

static void *passwd(FORM *form)
{
	if (E_INVALID_FIELD == form_validation(form)) {
		return NULL;
	}
	char sql[100];
	sprintf(sql, "select value from config where key = 'passwd'");
	db_select_1_row(get_db_main(), sql, 1, SELECT_TEXT, sql);

	/*--------------------------------------------------------------
	 *  field buffer is set full of chars(padding with 0x20)
	 *  if check with TYPE_ALNUM ...
	 *  so change the 1st 0x20 to 0
	 *-------------------------------------------------------------*/
	char *tmp = field_buffer(current_field(form), 0);
	spc2zr(tmp);
	if (strncmp(sql, tmp, LEN)) {
		print_notice("密码错误!!!");
		form_driver(form, REQ_CLR_FIELD);
	} else {
		print_notice("您确实是老板");
		data_analyse();
	}
	return NULL;
}

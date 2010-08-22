#include	<locale.h>
#include	"widget.h"

int ui_init()
{
	ncurses_init();
	win_init();
	main_menu();
        /* getch(); */
        return 0;
}

int ncurses_init()
{
        setlocale(LC_ALL, "zh_CN.UTF-8");
        initscr();
        noecho();
	/* cbreak(); */
        start_color();
        init_pair(1, COLOR_WHITE, COLOR_BLUE);
        curs_set(0);

	/* essential */
        refresh();
	return 0;
}

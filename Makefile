CC	:=	gcc
CFLAGS	:=	-std=gnu99 -O2 -g -c -I include/
LD	:=	ld
LIBS	:=	-lpthread -lncursesw -lmenuw -lformw -lsqlite3 
LOADER	:=	-dynamic-linker /lib/ld-linux.so.2

# if no '-lc' --
#   /usr/lib/crt1.o: In function `_start':
#   (.text+0xc):  undefined reference to `__libc_csu_fini'
#   (.text+0x11): undefined reference to `__libc_csu_init'
CRT	:=	/usr/lib/crt1.o /usr/lib/crti.o /usr/lib/crtn.o -lc
LDFLAGS :=	$(LOADER) $(CRT) $(LIBS) 

# all objs
OBJS	:= 	menu.o form.o widget.o win.o stdscr.o \
		event.o main_menu.o transact.o sales.o data_analyse.o get_today_bills.o passwd.o query.o \
		main.o aux.o stack.o \
		db.o
AOUT	:=	chafing
EXE	:=	wmj
DES	:=	/usr/bin

# search for '*.c' from current dir to 'db'
vpath	%.c	ui/view
vpath	%.c	ui/controler
vpath	%.c	glue
vpath	%.c	db

.PHONY	:all install clean

all	:$(AOUT)
$(AOUT)	:$(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o $@
$(OBJS) :%.o	:%.c
	$(CC) $(CFLAGS) $< -o $@

# '-' makes it ignore the retval whether failure or not
install :
	echo -e "#!/bin/bash\nexec chafing 2> /dev/null" > $(EXE)
	chmod a+x $(EXE)
	-cp $(AOUT) $(DES)
	-cp $(EXE) $(DES)

clean	:
	-rm -rf $(OBJS) $(AOUT) $(EXE)

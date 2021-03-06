# I am a comment, and I want to say that the variable CC will be
# the compiler to use.
CC=gcc
# Hey!, I am comment number 2. I want to say that CFLAGS will be the
# options I'll pass to the compiler.
CFLAGS= -c -Wall -std=c11 -Wextra -pedantic -Woverlength-strings -g -O0 -I /usr/local/include/libxml2/  -D_POSIX_C_SOURCE=200112L  -D_GNU_SOURCE 
# LIBS muss im lezten Schritt angehängt werden
LIBS = -L/usr/local/lib -L/usr/lib -lm -ljansson -lxml2 -llog4c

DEPS = req_data.h xml_handler.h struct.h update_service.h text.h cmd_args.h json_machine.h log_var.h
OBJ = main.o req_data.o xml_handler.o update_service.o cmd_args.o json_machine.o


all: RSSReader_1.0

RSSReader_1.0: $(OBJ)
	$(CC) $(OBJ) $(LIBS) -o $@ 


%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<





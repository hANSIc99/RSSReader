# I am a comment, and I want to say that the variable CC will be
# the compiler to use.
CC=gcc
# Hey!, I am comment number 2. I want to say that CFLAGS will be the
# options I'll pass to the compiler.
CFLAGS= -c -Wall -std=c11 -ansi -Wextra -pedantic -Woverlength-strings -g -O0 -I /usr/local/include/libxml2/ -D_POSIX_C_SOURCE=200112L -pedantic
# LIBS muss im lezten Schritt angehängt werden
LIBS = -L/usr/local/lib -lxml2 -lm -ldl
DEPS = req_data.h xml_handler.h struct.h update_service.h
OBJ = main.o req_data.o xml_handler.o update_service.o


all: RSSReader

RSSReader: $(OBJ)
	$(CC) $(OBJ) $(LIBS) -o $@


%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<





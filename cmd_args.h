/*
 * cmd_args.h
 * 
 * Copyright 2016 Stephan <stephanpc@stepohan-ubuntu64>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */
#ifndef _CMD_ARGS_H
#define _CMD_ARGS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "struct.h"


#define START_SUBADDR "/"

#define DOM 1
#define DOM_LONG 2
#define XML 3
#define HTTP 4
#define PRINT 5
#define UPDATE 6
#define BADARG -1

#define NKEYS (sizeof(lookuptable)/sizeof(type_struct))

int key_from_string(char *argv);
struct_adress **get_server_address(char *address_string,
				   struct_adress ** addr_ptr);
void set_server_adress_struct(const char *domain, const char *request,
			      struct_adress * s_addr);
void test_arg(char **argv);
void handle_options(char **argv, int *argc, struct_adress ** addr_pointer);
void
next_arg(char **argv, int *argc,
	 struct_adress ** addr_pointer, uint8_t * counter, uint8_t option);
void set_default_options(struct_adress **str_addr);
#endif

/*
 * json_mashine.h
 * 
 * Copyright 2016 Stephan Avenwedde <stephanpc@stepohan-ubuntu64>
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

#ifndef _JSON_MASHINE_H
#define _JSON_MASHINE_H

#include <inttypes.h>
#include "struct.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <jansson.h>
#include <ctype.h>
#include <time.h>
#include <log4c.h>

void process_json(struct_news_list ** List, struct_adress ** address_options);
uint16_t u16_keywrd_counter(char *, char *);

extern log4c_category_t* log_tracer;
extern log4c_category_t* log_debug;
extern log4c_category_t* log_raw;
#endif

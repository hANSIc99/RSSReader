/*
 * update_service.h
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

#ifndef _UPDATE_SERVICE_H
#define _UPDATE_SERVICE_H

#include <inttypes.h>
#include "struct.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
void update_assistant(const uint16_t * milliseconds, const uint16_t * seconds);
void initial_update(struct_news_list ** List, uint8_t print_flag);
uint8_t check_for_updates(const struct_news_list * new_list,
			  const struct_news_list * old_list,
			  const uint16_t u16_intervall,
			  const uint8_t u8_print_flag);
void free_list(struct_news_list * list);

#endif

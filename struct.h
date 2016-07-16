/*
 * struct.h
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

#ifndef _STRUCT_H
#define _STRUCT_H

#include <stdint.h>
#include <time.h>

#define MAX_DOMAIN_LENGHT 200
#define MAX_REQUEST_LENGHT 500

#define true 1
#define false 0

typedef uint8_t bool;
/* s_news = Name des Struct */
typedef struct s_news {

	uint16_t position;

	char *title;
	char *description;
	char *link;

	struct s_news *next;
	struct s_news *previous;

} struct_news;
/* struct_news = Name für Variablendeklaration: struct_news VarName */

typedef struct s_news_list {

	/* Enhthät bei Nachrichten die neuste Nachricht in der Liste */
	struct s_news *start;
	struct s_news *end;

} struct_news_list;

typedef struct s_adress {

	char *s_domain;
	char *s_request;
	char *s_raw_string;

	bool b_print;
	bool b_dom_parser;
	bool b_update;
	bool b_xml;
	bool b_http;
	bool b_json;

	char *search_keyword;
	uint16_t u16_update_interval_seconds;

} struct_adress;

typedef struct {
	char *key;
	int val;

} type_struct;

#endif

/*
 * xml_handler.h
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

#ifndef _XML_HANDLER
#define _XML_HANDLER

#include <stdint.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <libxml/xmlreader.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <log4c.h>
#include "struct.h"

struct_news_list *load_data(struct_adress * meta_info);
int dom_parser(const char *content, int length, struct_news_list * list_ptr);

void get_dom_objects(xmlNode * a_node, struct_news_list * list_ptr);
char *get_temp_string(char *xml_string, int startzeichen, int str_lenght);
char *get_server_info(char *xml_string, int startzeichen);
uint32_t get_starttag(const char *xml_string, const char *start_tag);
char *get_rss_tag(char *temp_string, char *end_tag, const uint32_t *str_lenght);
void append(struct_news ** lst, uint16_t * position, char *title, char *link,
	    char *description, char *tmp_pub_date, struct_news_list * list_ptr);

void dom_pub_date(xmlNode *cur_node);
void dom_title(xmlNode *cur_node);
void dom_link(xmlNode *cur_node);
uint8_t dom_description(xmlNode *cur_node);

extern log4c_category_t *log_tracer;
extern log4c_category_t *log_debug;
extern log4c_category_t *log_raw;
#endif

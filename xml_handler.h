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
#include "struct.h"
struct_news_list * load_data(char *xml_string);
int example3Func(const char *content, int length);

void print_element_names(xmlNode * a_node);
char * get_temp_string(char *xml_string, int startzeichen, int gesamtlaenge, int str_lenght);
char * get_server_info(char *xml_string, int startzeichen);
int get_starttag(const char * xml_string,const char * start_tag, const int *gesamtlaenge);
char * get_rss_tag(char *temp_string, char *end_tag, const int *str_lenght);
void append(struct_news **lst, uint16_t *position, char * title, char * link, char * description);



#endif

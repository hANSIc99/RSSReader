/*
 * json_mashine.c
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

#include "json_machine.h"

void process_json(struct_news_list ** List, struct_adress ** address_options)
{
JSON_INDENT(4);
JSON_COMPACT ;
	printf("json_mashine is here");
json_t *j_object, *j_string_1, *j_string_2;  
j_object = json_object();
j_string_1 = json_string("HALLO FROM JSON");
j_string_2 = json_string("Hier ist string 2");

json_object_set(j_object, "string_1",j_string_1);
json_object_set(j_object, "string_2", j_string_2);

printf("\n\n%s\n", json_dumps(j_object, JSON_INDENT(4))); 
}

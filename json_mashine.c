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

#include "json_mashine.h"

void process_json(struct_news_list ** List, struct_adress ** address_options)
{


	uint16_t u16_result_1;
	json_t *root, *js_keyword, *js_src_domain;
	root = json_object();

	printf("\n%s\n", (*address_options)->search_keyword);

	if((*address_options)->search_keyword != NULL){
	js_keyword = json_string((*address_options)->search_keyword);
	}
	if((*address_options)->s_domain != NULL){
		js_src_domain = json_string((*address_options)->s_domain);
	}


	json_object_set(root, "keyword_1", js_keyword);
	json_object_set(root, "source", js_src_domain);
	
	printf("\n%s\n", json_dumps(root, JSON_INDENT(4)));
	printf("\nNumber of elements in the object: %d\n", json_object_size(root));
}

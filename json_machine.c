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


	uint16_t u16_result_1;
	json_t *root, *js_keyword, *js_src_domain, *js_pub_date;
	struct_news *temp_pointer;
	root = json_object();

	if((*address_options)->search_keyword != NULL){
		js_keyword = json_string((*address_options)->search_keyword);
		printf("\nKeywors set!" );
	}
	if((*address_options)->s_domain != NULL){
		js_src_domain = json_string((*address_options)->s_domain);
	}
	if((*List)->start->pub_date != NULL){
		printf("\npub_date %s\n", (*List)->start->pub_date);
		js_pub_date = json_string((*List)->start->pub_date);
	}

	json_object_set(root, "keyword_1", js_keyword);
	json_object_set(root, "source", js_src_domain);
	json_object_set(root, "pub_date", js_pub_date);
	printf("\n%s\n", json_dumps(root, JSON_INDENT(4)));
	printf("\nNumber of elements in the object: %d\n", json_object_size(root));


	for (temp_pointer = (*List)->end; temp_pointer != NULL;
	     temp_pointer = temp_pointer->previous) {

			printf("\nTitle No.: %d : %s\n", temp_pointer->position,
			       temp_pointer->title);
			printf("\nLink: %s\n", temp_pointer->link);

			if (temp_pointer->description != NULL) {
				printf("\nDescription: %s\n\n",
				       temp_pointer->description);
			} else {
				printf
				    ("\nDescription: No description available\n\n");
			}
		
	}
}

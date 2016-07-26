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

	uint16_t u16_result_1, u16_match_counter;
	json_t *root, *js_keyword, *js_src_domain, *js_pub_date, *js_data;
	char *keyword_ptr, *keyword_tmp;
	struct_news *temp_pointer;
	js_data = json_object();
	root = json_object();


	json_object_set(root, "PRGRM", json_string("RSSReader"));

	if((*address_options)->search_keyword != NULL){
		js_keyword = json_string((*address_options)->search_keyword);
		json_object_set(js_data, "keyword_1", js_keyword);
	}
	if((*address_options)->s_domain != NULL){
		js_src_domain = json_string((*address_options)->s_domain);
		json_object_set(js_data, "source", js_src_domain);
	}
	if((*List)->start->pub_date != NULL){
		js_pub_date = json_string((*List)->start->pub_date);
		json_object_set(js_data, "pub_date", js_pub_date);
	}

	json_object_set(root, "data", js_data);

	printf("\n%s\n", json_dumps(root, JSON_INDENT(4)));
	printf("\nNumber of elements in the object: %d\n", json_object_size(root));

	keyword_ptr = NULL;

	for (temp_pointer = (*List)->end; temp_pointer != NULL;
	     temp_pointer = temp_pointer->previous) {

			printf("\nTitle No.: %d : %s\n", temp_pointer->position,
			       temp_pointer->title);

			if (temp_pointer->description != NULL) {

#if 1
			compare_strings(temp_pointer->description, (*address_options)->search_keyword);	
#endif
			



				printf("\ncounter: %d\n", u16_match_counter);
				keyword_ptr = NULL;
			
			} else {
				printf
				    ("\nDescription: No description available\n\n");
			}
		
	}
}
void compare_strings(char* string, char *keyword){
uint64_t diffrence;
char * ptr, *tmp_string;

tmp_string = string;	


ptr = strstr(tmp_string, keyword);

printf("\nstring: %u\n", tmp_string);
printf("\npointer: %u\n", ptr);

/* funktioniert */

while(ptr  != NULL)
{


tmp_string = ptr + strlen(keyword) +1;
				printf("\nkeyword found\n");

printf("\nptr: %u\n", ptr);
printf("\nstring: %u\n", tmp_string);
ptr = strstr(tmp_string, keyword);
}  






}

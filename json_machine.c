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

/*
 * u16_match_counter = 0 -> discription available, no keyword found
 * u16_match_counter = 1 -> 1 (!) match found !
 */

void process_json(struct_news_list ** List, struct_adress ** address_options)
{

	uint16_t u16_match_counter;
	json_t *root, *js_keyword, *js_src_domain, *js_pub_date, *js_data;
	char *keyword_ptr;
	struct_news *temp_pointer;

	u16_match_counter = 0;
	js_data = json_object();
	root = json_object();

	json_object_set(root, "PRGRM", json_string("RSSReader"));

	if ((*address_options)->search_keyword != NULL) {
		js_keyword = json_string((*address_options)->search_keyword);
		json_object_set(js_data, "keyword_1", js_keyword);
	}
	if ((*address_options)->s_domain != NULL) {
		js_src_domain = json_string((*address_options)->s_domain);
		json_object_set(js_data, "source", js_src_domain);
	}
	if ((*List)->start->pub_date != NULL) {
		js_pub_date = json_string((*List)->start->pub_date);
		json_object_set(js_data, "pub_date", js_pub_date);
	}

	json_object_set(root, "data", js_data);

	printf("\n%s\n", json_dumps(root, JSON_INDENT(4)));
	printf("\nNumber of elements in the object: %d\n",
	       (int)json_object_size(root));

	keyword_ptr = NULL;

	for (temp_pointer = (*List)->end; temp_pointer != NULL;
	     temp_pointer = temp_pointer->previous) {

		printf("\nTitle No.: %d : %s\n", temp_pointer->position,
		       temp_pointer->title);

		if ((keyword_ptr = temp_pointer->description) != NULL) {

			u16_match_counter =
			    u16_keywrd_counter(keyword_ptr,
					       (*address_options)->
					       search_keyword);

			printf("\nkeyword_counter: %d\n", u16_match_counter);

		} else {
			printf("\nDescription: No description available\n\n");
		}
		u16_match_counter = 0;

	}
}

uint16_t u16_keywrd_counter(char *source, char *keyword)
{

	uint16_t u16_match_counter = 0;
	char pre_src;
	char post_src;

	while ((source = strstr(source, keyword)) != NULL) {
		pre_src = *(source - 1);
		post_src = *(source + strlen(keyword));

		if ((pre_src == ' ') && (post_src == ' ')) {
			printf("\nKeyword found alone\n");
			u16_match_counter++;
		}
		source = source + strlen(keyword) + 1;

	}

	return u16_match_counter;
}

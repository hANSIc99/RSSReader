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

	uint16_t u16_match_counter[MAX_SEARCHKEYWORDS];
	uint8_t u8_keyword_counter = 0;
	json_t *root, *js_keyword, *js_tmp,
	    *js_data, *js_sys_time, *js_kwrd_array;
	char *keyword_ptr;
	time_t sys_time;
	struct_news *temp_pointer;
	log4c_category_log(log_tracer, LOG4C_PRIORITY_TRACE,
			   "%s: %s() -> function entered, set keyword matchcounter-array to zero",
			   (*address_options)->s_program_name, __func__);

	for (u8_keyword_counter = 0; u8_keyword_counter < MAX_SEARCHKEYWORDS;
	     ++u8_keyword_counter) {
		u16_match_counter[u8_keyword_counter] = 0;
	}

	js_data = json_object();
	root = json_object();
	js_kwrd_array = json_array();

	json_object_set(root, "PRGRM",
			json_string((*address_options)->s_program_name));

	json_object_set(root, "data", js_data);

	time(&sys_time);
	js_sys_time = json_integer(sys_time);

	if ((*address_options)->s_domain) {
		js_tmp = json_string((*address_options)->s_domain);
		json_object_set(js_data, "source", js_tmp);
	}
	if ((*List)->start->pub_date) {
		js_tmp = json_string((*List)->start->pub_date);
		json_object_set(js_data, "pub_date", js_tmp);
	}
	if ((*address_options)->s_customer) {
		js_tmp = json_string((*address_options)->s_customer);
		json_object_set(js_data, "customer", js_tmp);
	}

	json_object_set(js_data, "sys_time", js_sys_time);

	keyword_ptr = NULL;

	for (temp_pointer = (*List)->end;
	     temp_pointer != NULL; temp_pointer = temp_pointer->previous) {

		/* test if a describtion exists */

		if ((keyword_ptr = temp_pointer->description)) {
			/* check how many keywords are passed as arguments */
			for (u8_keyword_counter = 0;
			     ((*address_options)->search_keyword
			      [u8_keyword_counter] != NULL);
			     ++u8_keyword_counter) {
				/* count the matches for the specific keywords */
				u16_match_counter[u8_keyword_counter] +=
				    u16_keywrd_counter(keyword_ptr,
						       (*address_options)->
						       search_keyword
						       [u8_keyword_counter]);

				log4c_category_log(log_raw,
						   LOG4C_PRIORITY_TRACE,
						   "%s: %s() -> find keywords in describtion of topic %d:\n%s",
						   (*address_options)->
						   s_program_name, __func__,
						   temp_pointer->position,
						   temp_pointer->title);
				log4c_category_log(log_raw,
						   LOG4C_PRIORITY_TRACE,
						   "%s: %s() -> matches found %d",
						   (*address_options)->
						   s_program_name, __func__,
						   u16_match_counter
						   [u8_keyword_counter]);
			}
		}

	}

	/* Creating the JSON Array with the results of the search function 
	 */

	log4c_category_log(log_tracer, LOG4C_PRIORITY_TRACE,
			   "%s: %s() -> creating JSON array with the results of the searching function",
			   (*address_options)->s_program_name, __func__);

	for (u8_keyword_counter = 0;
	     ((*address_options)->search_keyword[u8_keyword_counter]) != NULL;
	     ++u8_keyword_counter) {

		js_keyword = json_object();
		json_object_set(js_keyword, "s_keyword",
				json_string((*address_options)->search_keyword
					    [u8_keyword_counter]));

		js_tmp = json_integer((json_int_t)
				      u16_match_counter[u8_keyword_counter]);
		json_object_set(js_keyword, "u16_result", js_tmp);

		json_array_append(js_kwrd_array, js_keyword);

	}

	json_object_set(js_data, "result", js_kwrd_array);

	log4c_category_log(log_tracer, LOG4C_PRIORITY_TRACE,
			   "%s: %s() -> print out the JSON-Object",
			   (*address_options)->s_program_name, __func__);
	printf("\n%s\n", json_dumps(root, JSON_INDENT(4)));

}

uint16_t u16_keywrd_counter(char *source, char *keyword)
{

	uint16_t u16_match_counter = 0;
	char pre_src;
	char post_src;

	while ((source = strcasestr(source, keyword))) {
		/* if the substring is found */
		pre_src = *(source - 1);
		post_src = *(source + strlen(keyword));

		if ((pre_src == ' ')
		    && ((post_src == ' ') || (post_src == ',')
			|| (post_src == '.') || (post_src == 's'))) {
			u16_match_counter++;
		}
		source = source + strlen(keyword) + 1;

	}

	return u16_match_counter;
}

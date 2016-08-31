/*
 * update_service.c
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

#include "update_service.h"
#define DELAY_MILLI 500;
#define DELAY_CON_LOST 10;

void initial_update(struct_news_list ** List, struct_adress ** address_options)
{

	struct_news *temp_pointer;

	for (temp_pointer = (*List)->end;
	     temp_pointer != NULL; temp_pointer = temp_pointer->previous) {

		if (((*address_options)->b_print == true)
		    && ((*address_options)->b_json == false)) {
			printf("\nTitle No.: %d : %s\n",
			       temp_pointer->position, temp_pointer->title);
			printf("\nLink: %s\n", temp_pointer->link);

			if (temp_pointer->description != NULL) {
				printf
				    ("\nDescription: %s\n\n",
				     temp_pointer->description);
			} else {
				printf
				    ("\nDescription: No description available\n\n");
			}
		}
	}
	if ((*address_options)->b_json == true) {
		/* here the JSON-Object is build */
		log4c_category_log(log_tracer, LOG4C_PRIORITY_TRACE,
				   "%s: %s() -> call json-building function",
				   (*address_options)->s_program_name,
				   __func__);

		process_json(List, address_options);
	}

}

uint8_t
check_for_updates(const struct_news_list * new_list,
		  const struct_news_list * old_list,
		  struct_adress ** address_options)
{
/* u16_intervall and u8_print flag have to be replaced */
	uint16_t delay_milliseconds = DELAY_MILLI
	    uint16_t delay_connection_lost = DELAY_CON_LOST
	    struct_news * str_ptr;

	if (old_list != NULL) {

		for (str_ptr = new_list->start;
		     str_ptr != NULL; str_ptr = str_ptr->next) {


log4c_category_log(log_tracer, LOG4C_PRIORITY_DEBUG, "%s: %s() -> result of strcmp: %d",
				   (*address_options)->s_program_name,
				   __func__, strncmp((str_ptr->title),
					     (old_list->start->title),
					     (strlen(new_list->start->title))));

			if (strcmp(str_ptr->title, old_list->start->title) != 0) {

				if ((*address_options)->b_print != 0) {

					if (str_ptr->title != NULL) {
						printf
						    ("\nNew headline:\n%s\n\n",
						     str_ptr->title);
					}
					if (str_ptr->link != NULL) {
						printf
						    ("\nLink: %s\n",
						     str_ptr->link);
					}
					if (str_ptr->description != NULL) {
						printf
						    ("\nDescription: %s\n\n",
						     str_ptr->description);
					}
				}
#if 0
				exit(1);
#endif
			} else {
#if 0
				printf
				    ("\nupdate_service: Keine neue Nachricht gefunden\n");
#endif
				/* for-loop verlassen */
				break;
			}

		}

		update_assistant(&delay_milliseconds,
				 &
				 (*address_options)->
				 u16_update_interval_seconds);
		/* If old_list was not NULL */
		return 1;
	} else {

		update_assistant(&delay_milliseconds, &delay_connection_lost);
		return 0;
	}
/* if old_list was 0 */

}

void update_assistant(const uint16_t * milliseconds, const uint16_t * seconds)
{

	uint32_t u32_nano_seconds;
	struct timespec sleeptime, remainingtime;

	u32_nano_seconds = *milliseconds * 1000 * 1000;
	sleeptime.tv_nsec = u32_nano_seconds;
	sleeptime.tv_sec = *seconds;

	nanosleep(&sleeptime, &remainingtime);

}

void free_list(struct_news_list * list)
{
	struct_news *tmp_ptr, *start_ptr;
	start_ptr = list->start;

	while (start_ptr != NULL) {
		tmp_ptr = start_ptr;
		start_ptr = start_ptr->next;
		free(tmp_ptr->title);
		free(tmp_ptr->description);
		free(tmp_ptr->link);
/*
		free(tmp_ptr->pub_date);
*/
		free(tmp_ptr);
	}

	free(list);
}

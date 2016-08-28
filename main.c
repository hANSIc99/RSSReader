/*
 * main.c
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

/*! \file main.c
 *  \brief From here all other functions are called
 * 
 * In the while loop of the main function, all 40 seconds an update of the 
 * news-list is requested and compared to the outdated one.
 */

/*! \fn int key_from_string(char *argv)
  \brief Compares the arguments.
  \param *argv The name of the descriptor.

 */

#define POSIX_C_SOURCE 200809L

#include <stdio.h>
#include "req_data.h"
#include "xml_handler.h"
#include "struct.h"
#include "update_service.h"
#include <inttypes.h>
#include <time.h>
#include "text.h"
#include "cmd_args.h"
#include <log4c.h>
#define DEBUG 0
#define PRINT_OUT 1
#define DELAY_SEC 10

int delay_seconds = DELAY_SEC;

/* todo's:
 * 
 *implement the json output   
 */

int main(int argc, char **argv)
{
	struct_adress *rss_addres_options = NULL;

	uint8_t u8_update_flag = 1;
	uint8_t u8_keyword_count;
	struct_news_list *List1, *List2;

	List1 = NULL;
	List2 = NULL;

	rss_addres_options = malloc(sizeof(struct_adress));
	memset(rss_addres_options, 0, sizeof(struct_adress));
	
log4c_init();



	set_default_options(&rss_addres_options);

	LIBXML_TEST_VERSION 

		/* read command line options */

		handle_options(argv, &argc, &rss_addres_options);

	if (rss_addres_options->b_print) {
		printf("%s", start_licence);
	}

	if (rss_addres_options) {
		/* req_svr_ptr holds the raw data from the server */
		if (DEBUG) {
			printf("\nData loaded!\n");
		}
		req_server(rss_addres_options);
		List1 = load_data(rss_addres_options);
	} else {
		if (DEBUG) {
		}
	}

	/* First time reading; news at last */
	if (List1) {

		initial_update(&List1, &rss_addres_options);

	} else {
		printf("\nRecall server.....\n");
	}

#if 1

	while (rss_addres_options->b_update) {

		if (u8_update_flag != 0) {

			u8_update_flag = 0;

			if (DEBUG) {
				printf("\nFirst Test");
			}
			req_server(rss_addres_options);
			List2 = load_data(rss_addres_options);

			check_for_updates(List2, List1, &rss_addres_options);

			free_list(List1);

		}

		else {
			if (DEBUG) {
				printf("\nSeconds Test");
			}
			u8_update_flag = 1;
			req_server(rss_addres_options);
			List1 = load_data(rss_addres_options);

			check_for_updates(List1, List2, &rss_addres_options);
			free_list(List2);

		}

	}
#endif

	free(rss_addres_options->s_domain);
	free(rss_addres_options->s_request);
	free(rss_addres_options->s_raw_string);
	free(rss_addres_options->s_customer);
	while (rss_addres_options->search_keyword[u8_keyword_count]) {

		free(rss_addres_options->search_keyword[u8_keyword_count]);
		++u8_keyword_count;

	}
	free(rss_addres_options);
	free_list(List1);

	return 0;
}

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
#include "log_var.h"

#define PRINT_OUT 1
#define DELAY_SEC 10

int delay_seconds = DELAY_SEC;

/* todo's:
 * 
 *implement the json output   
 */
extern log4c_category_t *log_tracer;
extern log4c_category_t *log_debug;
extern log4c_category_t *log_raw;

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

	rss_addres_options->s_program_name = strdup(*argv);

	set_default_options(&rss_addres_options);

	LIBXML_TEST_VERSION
	    /* read command line options */
	    handle_options(argv, &argc, &rss_addres_options);

	/* log4c initialisation can only take place after the program arguments are processed */

	if (log4c_init()) {
		printf("\nlog4c_init() failed\n");
		exit(1);
	} else {
		log_tracer = log4c_category_get("tracer");
		log_debug = log4c_category_get("debug");
		log_raw = log4c_category_get("raw_data");

		log4c_category_log(log_tracer, LOG4C_PRIORITY_TRACE,
				   "%s: log4c initialized",
				   rss_addres_options->s_program_name);

	}
	if (rss_addres_options->b_print) {
		printf("%s", start_licence);
	}

	if (rss_addres_options) {
		/* req_svr_ptr holds the raw data from the server */
		log4c_category_log(log_tracer, LOG4C_PRIORITY_TRACE,
				   "%s: %s() -> arguments processed",
				   rss_addres_options->s_program_name,
				   __func__);
		req_server(rss_addres_options);
		List1 = load_data(rss_addres_options);
	} else {
		log4c_category_log(log_debug, LOG4C_PRIORITY_ERROR,
				   "%s: %s() -> adress struct empty",
				   rss_addres_options->s_program_name,
				   __func__);
	}

	/* First time reading; news at last */
	if (List1) {

		log4c_category_log(log_tracer, LOG4C_PRIORITY_TRACE,
				   "%s: %s() -> prints the content or the json on stdout",
				   rss_addres_options->s_program_name,
				   __func__);
		initial_update(&List1, &rss_addres_options);

	} else {
		log4c_category_log(log_tracer, LOG4C_PRIORITY_TRACE,
				   "%s: %s() -> no content available, recalling the server",
				   rss_addres_options->s_program_name,
				   __func__);

		if (rss_addres_options->b_print) {
			printf("\nRecall server.....\n");
		}
	}

	while (rss_addres_options->b_update) {

		if (u8_update_flag) {

			u8_update_flag = 0;

			log4c_category_log(log_tracer, LOG4C_PRIORITY_TRACE,
					   "%s: %s() -> first update request",
					   rss_addres_options->s_program_name,
					   __func__);

			req_server(rss_addres_options);
			List2 = load_data(rss_addres_options);

			check_for_updates(List2, List1, &rss_addres_options);

			free_list(List1);

		}

		else {

			log4c_category_log(log_tracer, LOG4C_PRIORITY_TRACE,
					   "%s: %s() -> second update request",
					   rss_addres_options->s_program_name,
					   __func__);

			u8_update_flag = 1;
			req_server(rss_addres_options);
			List1 = load_data(rss_addres_options);

			check_for_updates(List1, List2, &rss_addres_options);
			free_list(List2);

		}

	}

	if (log4c_fini()) {
		printf("log4c_fini() failed");
	}

/* free allocated memory */
	log4c_category_log(log_tracer, LOG4C_PRIORITY_TRACE,
			   "%s: %s() -> free allocated memory",
			   rss_addres_options->s_program_name, __func__);

	free(rss_addres_options->s_domain);
	free(rss_addres_options->s_request);
	free(rss_addres_options->s_raw_string);
	free(rss_addres_options->s_customer);
	free(rss_addres_options->s_program_name);
	u8_keyword_count = 0;
	while (rss_addres_options->search_keyword[u8_keyword_count]) {

		free(rss_addres_options->search_keyword[u8_keyword_count]);
		++u8_keyword_count;

	}
	free(rss_addres_options);
	free_list(List1);

	return 0;
}

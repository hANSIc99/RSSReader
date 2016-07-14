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
#define DEBUG 0
#define PRINT_OUT 1
#define DELAY_SEC 10

int delay_seconds = DELAY_SEC;

/* todo's:
 * 
 *   implement to upper function
 * 
 *   http://www.cplusplus.com/reference/cctype/toupper/
 * 
 * 
 */

int
main (int argc, char **argv)
{
  struct_adress *rss_addres_options = NULL;

  char *req_svr_ptr;
  uint8_t update_flag = 1;
  struct_news_list *List1, *List2;

  List1 = NULL;
  List2 = NULL;
  
  
  rss_addres_options = malloc(sizeof(struct_adress));
  memset(rss_addres_options, 0, sizeof(struct_adress));
  
  set_default_options(&rss_addres_options);
  /* rss_addres_options->b_update = true; */

  LIBXML_TEST_VERSION
    handle_options (argv, &argc, &rss_addres_options);

  printf ("%s", start_licence);

  if ((req_svr_ptr =
       req_server (rss_addres_options)) != NULL)
    {
      /* req_svr_ptr holds the raw data from the server */
      if (DEBUG)
	printf ("\nData loaded!\n");

      List1 = load_data (req_svr_ptr);
    }
  else
    {
      if (DEBUG)
	printf ("\nKeine Daten vorhanden: %s\n",
		req_svr_ptr);
    }

  /* First time reading; news at last */
  if (List1 != NULL)
    {
	
      initial_update (&List1, rss_addres_options->b_print);

    }
  else
    {
      printf ("\nRecall server.....\n");
    }



#if 1

  while (rss_addres_options->b_update)
    {

      if (update_flag != 0)
	{

	  update_flag = 0;

	  if (DEBUG)
	    printf ("\nFirst Test");

	  List2 =
	    load_data (req_server (rss_addres_options));

	  check_for_updates (List2, List1,rss_addres_options->u16_update_interval_seconds, 
			     rss_addres_options->b_print);

	  free_list (List1);

	}

      else
	{
	  if (DEBUG)
	    printf ("\nSeconds Test");
	  update_flag = 1;

	  List1 =
	    load_data (req_server (rss_addres_options));

	  check_for_updates (List1, List2, delay_seconds,
			     rss_addres_options->b_print);
	  free_list (List2);

	}

    }
#endif
	
  free(rss_addres_options->s_domain);
  free(rss_addres_options->s_request);
  free(rss_addres_options->search_keyword);
  free(rss_addres_options);
  free_list(List1);

  return 0;
}

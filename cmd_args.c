/*
 * cmd_args.c
 * 
 * Copyright 2016 Stephan <stephanpc@stepohan-ubuntu64>
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

#include "cmd_args.h"

#define DEBUG 0

uint8_t option_counter = false;

static type_struct lookuptable[] = {
	{"DOM", DOM},
	{"PARSER=DOMAIN-OBJECT-MODEL", DOM_LONG},
	{"XML", XML},
	{"HTTP", HTTP},
	{"PRINT", PRINT},
	{"UPDATE", UPDATE},
	{"KEYWORD", KEYWORD},
	{"JSON", JSON}
};

void handle_options(char **argv, int *argc, struct_adress ** addr_pointer)
{

	/* hier prüfen wie viele adressen eingegeben wurden */

	char **test_val;
	argv++;

	if (*argv != NULL) {
		/* option counter nicht global machen */
		test_arg(argv);

		if (option_counter != 0) {

			switch (key_from_string(*argv)) {
			case DOM:
				printf("\noption DOM = true");
				(*addr_pointer)->b_dom_parser = true;
				option_counter = 0;
				handle_options(argv, argc, addr_pointer);
				break;
			case DOM_LONG:
				printf("\noption DOM_LONG = true");
				(*addr_pointer)->b_dom_parser = true;
				option_counter = 0;
				handle_options(argv, argc, addr_pointer);
				break;
			case XML:
				printf("\noption XML = true\n");
				option_counter = 0;
				handle_options(argv, argc, addr_pointer);
				break;
			case HTTP:
				printf("\noption HTTP = true\n");
				option_counter = 0;
				handle_options(argv, argc, addr_pointer);
				break;
			case PRINT:
				printf("\noption PRINT = true\n");
				(*addr_pointer)->b_print = true;
				option_counter = 0;
				handle_options(argv, argc, addr_pointer);
				break;
			case UPDATE:
				printf("\noption UPDATE = true\n");
				test_val = argv;
				test_val++;
				(*addr_pointer)->b_update = true;
				if ((*test_val) != NULL) {
					(*addr_pointer)->
					    u16_update_interval_seconds =
					    strtol((*test_val), NULL, 10);
				}
				if ((*addr_pointer)->u16_update_interval_seconds
				    == 0) {
					printf
					    ("\nNo correct interval found, default will be 60 seconds.\n");
					(*addr_pointer)->
					    u16_update_interval_seconds = 60;
				} else {
					printf
					    ("\nUpdate interval: %d seconds\n",
					     ((*addr_pointer)->
					      u16_update_interval_seconds));
					argv++;
				}
				option_counter = 0;
				handle_options(argv, argc, addr_pointer);
				break;
			case KEYWORD:
				printf("\noption KEYWORD = true\n");
				test_val = argv;
				test_val++;
				if (((*test_val) != NULL)
				    && (((*test_val)[0]) != '-')) {
					(*addr_pointer)->search_keyword =
					    strdup(*test_val);
					argv++;
					printf("\nKeyword found: %s\n",
					       (*addr_pointer)->search_keyword);
				} else {
					printf("\nError, no keyword found.\n"
					       "The command is executed by -keyword WORDXY");
				}
				option_counter = 0;
				handle_options(argv, argc, addr_pointer);
				break;
			case JSON:
				printf("\noption JSON = true\n");
				(*addr_pointer)->b_json = true;
				option_counter = 0;
				handle_options(argv, argc, addr_pointer);
				break;

			case BADARG:
				printf
				    ("Command \"%s\" not found.\nRSSReader -help list available commands and a short manual\n",
				     (*argv));
				exit(1);
				break;
			}

		} else {
			/* recursive implementation */

			addr_pointer = get_server_address(*argv, addr_pointer);
			if (addr_pointer == NULL) {
				printf("\nServer address query failed\n");
				exit(1);
			}

		}

	} else {
		printf("\nAt least a valid server address is needed!\n");
		exit(1);
	}

}

void test_arg(char **argv)
{
	if (((*argv)[0]) == '-') {

		option_counter++;
		*argv = (*argv) + 1;
		test_arg(argv);

	}
}

int key_from_string(char *argv)
{
	uint8_t i;
	char *str;
	str = malloc(strlen(argv) * sizeof(char));
	memset(str, 0, (strlen(argv) * sizeof(char)));

	/* convert args to upper chars */

	while (argv[i]) {
		str[i] = toupper(argv[i]);
		i++;
	}

	for (i = 0; i < NKEYS; i++) {
		type_struct *typ = &lookuptable[i];
		if (strcmp(typ->key, str) == 0) {
			free(str);
			return typ->val;
		}
	}
	free(str);
	return BADARG;
}

struct_adress
    ** get_server_address(char *address_string, struct_adress ** addr_ptr)
{
	uint16_t u16_sub_addr;
	char *domain, *req;

	if (DEBUG)
		printf("\n\nArg 1: %s\n", address_string);

	u16_sub_addr = strcspn(address_string, START_SUBADDR);
	if (DEBUG)
		printf("\n\nStarttag at %d\n", u16_sub_addr);

	if ((domain = strtok(address_string, START_SUBADDR)) != NULL) {

		if ((req = strtok(NULL, " ")) != NULL) {

			set_server_adress_struct(domain, req, *addr_ptr);

		} else {
			return NULL;
		}

	}

	else {
		return NULL;
	}

	return addr_ptr;

}

void
set_server_adress_struct(const char *domain,
			 const char *request, struct_adress * s_addr)
{

	s_addr->s_domain = strdup(domain);
	s_addr->s_request = strdup(request);

}

void set_default_options(struct_adress ** str_addr)
{

	(*str_addr)->b_update = false;
	(*str_addr)->b_dom_parser = true;

}

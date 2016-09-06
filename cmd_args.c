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

static uint8_t u8_option_counter = false;
static uint8_t u8_keyword_counter;

static type_struct lookuptable[] = {
	{"DOM", DOM},
	{"PARSER=DOMAIN-OBJECT-MODEL", DOM_LONG},
	{"XML", XML},
	{"HTTP", HTTP},
	{"PRINT", PRINT},
	{"UPDATE", UPDATE},
	{"KEYWORD", KEYWORD},
	{"CUSTOMER", CUSTOMER},
	{"JSON", JSON}
};

void handle_options(char **argv, int *argc, struct_adress ** addr_pointer)
{

	/* hier prüfen wie viele adressen eingegeben wurden */

	char **test_val;
	argv++;
	if (*argv) {
		/* option counter nicht global machen */
		test_arg(argv);

		if (u8_option_counter) {

			switch (key_from_string(*argv)) {
			case DOM:
				if ((*addr_pointer)->b_print) {
					printf("\noption DOM = true");
				}
				(*addr_pointer)->b_dom_parser = true;
				u8_option_counter = 0;
				handle_options(argv, argc, addr_pointer);
				break;
			case DOM_LONG:
				if ((*addr_pointer)->b_print) {
					printf("\noption DOM_LONG = true");
				}
				(*addr_pointer)->b_dom_parser = true;
				u8_option_counter = 0;
				handle_options(argv, argc, addr_pointer);
				break;
			case XML:
				if ((*addr_pointer)->b_print) {
					printf("\noption XML = true\n");
				}
				u8_option_counter = 0;
				handle_options(argv, argc, addr_pointer);
				break;
			case HTTP:
				if ((*addr_pointer)->b_print) {
					printf("\noption HTTP = true\n");
				}
				u8_option_counter = 0;
				handle_options(argv, argc, addr_pointer);
				break;
			case PRINT:
				printf("\noption PRINT = true\n");
				(*addr_pointer)->b_print = true;
				u8_option_counter = 0;
				handle_options(argv, argc, addr_pointer);
				break;
			case UPDATE:
				if ((*addr_pointer)->b_print) {
					printf("\noption UPDATE = true\n");
				}
				test_val = argv;
				test_val++;
				(*addr_pointer)->b_update = true;
				if ((*test_val)) {
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
					     ((*addr_pointer)->u16_update_interval_seconds));
					argv++;
				}
				u8_option_counter = 0;
				handle_options(argv, argc, addr_pointer);
				break;
			case KEYWORD:
				if ((*addr_pointer)->b_print) {
					printf("\noption KEYWORD = true\n");
				}
				test_val = argv;
				test_val++;
				if ((*test_val)
				    && (((*test_val)[0]) != '-')) {
					(*addr_pointer)->search_keyword
					    [u8_keyword_counter] =
					    strdup(*test_val);
					argv++;
					if ((*addr_pointer)->b_print) {
						printf
						    ("\nKeyword found: %s\n",
						     (*addr_pointer)->search_keyword
						     [u8_keyword_counter]);
					}
					++u8_keyword_counter;
				} else {
					printf
					    ("\nError, no keyword found.\n"
					     "The command is executed by -keyword WORDXY");
				}
				u8_option_counter = 0;
				handle_options(argv, argc, addr_pointer);
				break;
			case JSON:
				if ((*addr_pointer)->b_print) {
					printf("\noption JSON = true\n");
				}
				(*addr_pointer)->b_json = true;
				u8_option_counter = 0;
				handle_options(argv, argc, addr_pointer);
				break;
			case CUSTOMER:
				if ((*addr_pointer)->b_print) {
					printf("\noption CUSTOMER = true\n");
				}
				test_val = argv;
				test_val++;
				if ((*test_val)
				    && (((*test_val)[0]) != '-')) {
					(*addr_pointer)->s_customer =
					    strdup(*test_val);
					argv++;
					if ((*addr_pointer)->b_print) {
						printf
						    ("\nCustomer found: %s\n",
						     (*addr_pointer)->
						     s_customer);
					}
				} else {
					printf
					    ("\nError, no keyword found.\n"
					     "The command is executed by -keyword WORDXY");
				}
				u8_option_counter = 0;
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

		u8_option_counter++;
		*argv = (*argv) + 1;
		test_arg(argv);

	}
}

int key_from_string(char *argv)
{
	uint8_t i = 0;
	uint16_t u16_str_lenght = 0;
	char *str;

	u16_str_lenght = strlen(argv);
	u16_str_lenght++;

	str = malloc(u16_str_lenght * sizeof(char));
	memset(str, 0, (u16_str_lenght * sizeof(char)));

#if  0				/* ----- #if 0 : If0Label_1 ----- */
	printf("\nHALLO : %s COUNTER: %d\n", (argv), u8_option_counter);
#endif				/* ----- #if 0 : If0Label_1 ----- */

	/* convert args to upper chars */

	while (argv[i]) {
		str[i] = toupper(argv[i]);
		i++;
	}
	
	str[i] = '\0' ;

	for (i = 0; i < NKEYS; i++) {
		type_struct *typ = &lookuptable[i];
		if (!(strcmp(typ->key, str))) {
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

	if ((domain = strtok(address_string, START_SUBADDR))) {

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

	(*str_addr)->b_print = false;
	(*str_addr)->b_dom_parser = true;
	(*str_addr)->b_update = false;
	(*str_addr)->b_xml = false;
	(*str_addr)->b_http = false;
	(*str_addr)->b_json = false;

}

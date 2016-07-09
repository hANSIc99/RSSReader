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

uint8_t option_counter = 0;

static type_struct lookuptable[] = {
	{"dom", DOM},
	{"parser=domain-object-model", DOM_LONG},
	{"A3", A3},
	{"A4", A4}
};

void handle_options(char **argv, int *argc, struct_adress ** addr_pointer)
{

	/* hier prüfen wie viele adressen eingegeben wurden */
	type_struct *type;

	argv++;

	if (*argv != NULL) {
		/* option counter nicht global machen */
		test_arg(argv);

		if (option_counter != 0) {

			switch (key_from_string(*argv)) {
			case DOM:

				type = &lookuptable[DOM - 1];
				*argv =
				    (*argv) + strlen(type->key) +
				    option_counter;
				option_counter = 0;
				printf("\ncase dom\n");
				handle_options(argv, argc, addr_pointer);
				break;
			case DOM_LONG:
				printf("\ncase dom_long\n");
				break;
			case A3:
				printf("\ncase A3\n");
				break;
			case A4:
				printf("\ncase A4\n");
				break;
			case BADARG:
				printf
				    ("RSSReader -help list available commands and a short manual\n");

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

	for (i = 0; i < NKEYS; i++) {
		type_struct *typ = &lookuptable[i];
		if (strcmp(typ->key, argv) == 0) {
			return typ->val;
		}
	}
	return BADARG;
}

struct_adress
    ** get_server_address(char *address_string, struct_adress ** addr_ptr)
{
	uint16_t u16_sub_addr, u16_addr_lenght;
	char *domain, *req;

	*addr_ptr = malloc(sizeof(struct_adress));
	memset(*addr_ptr, 0, sizeof(struct_adress));
	u16_addr_lenght = strlen(address_string);
	if (DEBUG)
		printf("\n\nArg 1: %s\n", address_string);

	u16_sub_addr = strcspn(address_string, START_SUBADDR);
	if (DEBUG)
		printf("\n\nStarttag at %d\n", u16_sub_addr);

	domain = malloc(u16_sub_addr * sizeof(char));
	req = malloc((u16_addr_lenght - u16_sub_addr) * sizeof(char));

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
set_server_adress_struct(const char *domain, const char *request,
			 struct_adress * s_addr)
{

	strncpy(s_addr->s_domain, domain, strlen(domain));
	strncpy(s_addr->s_request, request, strlen(request));

}

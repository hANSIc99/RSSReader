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
#define POSIX_C_SOURCE 200809L

#include <stdio.h>
#include "req_data.h"
#include "xml_handler.h"
#include "struct.h"
#include "update_service.h"
#include <inttypes.h>
#include <time.h>

#define DEBUG 0
#define PRINT 1
#define DELAY_SEC 10;

#define START_SUBADDR "/"
int delay_seconds = DELAY_SEC 
struct_adress *rss_address_temp = NULL;

void get_server_address(char* address_string);
void set_server_adress_struct(const char* domain, const char *request, struct_adress *s_addr);
/* todo's:
 * 
 *   get_temp string weiterarbeiten
 */
 
 
static int handle_options(char **argv, int *argc)
{
	
	/* hier prüfen wie viele adressen eingegeben wurden */
char * svr_addr;

argv++;

if(argv != NULL){
	svr_addr = *argv;

	get_server_address(svr_addr);	
	
	return 0;
	}
	else
	{
	return 1;
	}
}

void get_server_address(char* address_string){
	uint16_t u16_sub_addr, u16_addr_lenght;
	char *domain, *req;
	struct_adress *rss_address;
	
	
	rss_address = malloc(sizeof(struct_adress));
	
	/* Very important !!! */
	
	memset(rss_address, 0 ,sizeof(struct_adress));

	
	u16_addr_lenght = strlen(address_string);
	if(DEBUG)
	printf("\n\nArg 1: %s\n", address_string);
	
	u16_sub_addr = strcspn(address_string, START_SUBADDR);
	if(DEBUG)
	printf("\n\nStarttag at %d\n", u16_sub_addr);
	
	domain = malloc(u16_sub_addr * sizeof(char));
	req = malloc((u16_addr_lenght - u16_sub_addr) * sizeof(char));
	

	
	if((domain = strtok(address_string, START_SUBADDR)) != NULL){
		
		
		if((req = strtok(NULL, " ")) != NULL){
		
		set_server_adress_struct(domain, req, rss_address);
			
			
		}
		else{
			/* return NULL; */
		}
		
	}
	
	else{
	/* return NULL; */
	}
	
	rss_address_temp = rss_address;
	
	/* return rss_address;	*/
	
}



void set_server_adress_struct(const char* domain, const char *request, struct_adress *s_addr){

strncpy(s_addr->s_domain, domain, strlen(domain));
strncpy(s_addr->s_request, request, strlen(request));

}


int main(int argc, char **argv)
{

	char * req_svr_ptr;
	uint8_t update_flag = 1;
	uint8_t test_flag;
	const struct_news_list *List1, *List2;
	test_flag = 0;
	List1 = NULL;
	List2 = NULL;
	

	
	LIBXML_TEST_VERSION
	
	

	rss_address_temp = malloc(sizeof(struct_adress));

	
	
	if((argc > 1) && (argv != NULL)){		
		/* test if the arguments are correct */	
		
		test_flag = handle_options(argv, &argc);
		
		
	}
	else{
		/* the user needs help */
		printf("RSSReader -help list available commands and a short manual\n");
		return 1;
	}
	
	
	
	if((req_svr_ptr = req_server(rss_address_temp)) != NULL){
		/* req_svr_ptr holds the raw data from the server */
		if(DEBUG)
		printf("\nData loaded!\n");
	
		List1 = load_data(req_svr_ptr);
	}
	else{
		if(DEBUG)
		printf("\nKeine Daten vorhanden: %s\n", req_svr_ptr);
	}
	

	/* First time reading; news at last */
	if(List1 != NULL){
	
	initial_update(&List1, PRINT);
	
	}
	else{
		printf("\nRecall server.....\n");
	}
	
	
	
	#if 1
	while(1){
		
		if(update_flag != 0){
	
			update_flag = 0;
			if(DEBUG)		
			printf("\nFirst Test");
			
			List2 = load_data(req_server(rss_address_temp));

			check_for_updates(List2, List1, delay_seconds, PRINT);

		}
	
		else {
			if(DEBUG)
			printf("\nSeconds Test");
			update_flag = 1;
			
			List1 = load_data(req_server(rss_address_temp));			
			
			check_for_updates(List1, List2, delay_seconds, PRINT);
			
		}

		
	}
	#endif 
	free(rss_address_temp->s_domain);
	free(rss_address_temp->s_request);
	free(rss_address_temp);

	return 0;
}






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
#define DEBUG 0


void initial_update(const struct_news_list **List, uint8_t print_flag){
	
struct_news  *temp_pointer; 
	


for(temp_pointer = (*List)->end; temp_pointer != NULL; temp_pointer = temp_pointer->previous){
	
	if(print_flag){
				printf("\nTitle No.: %d : %s\n", temp_pointer->position, temp_pointer->title); 
				printf("\nLink: %s\n", temp_pointer->link);

				if(temp_pointer->description != NULL){
					printf("\nDescription: %s\n\n", temp_pointer->description);
				}
				else{
					printf("\nDescription: No description available\n\n");
				}
			}
}


}

uint8_t check_for_updates(const struct_news_list *new_list,const struct_news_list *old_list,const uint16_t u16_intervall,const uint8_t u8_print_flag ){
	
uint16_t delay_milliseconds = DELAY_MILLI 	
uint16_t delay_connection_lost = DELAY_CON_LOST 
struct_news *str_ptr;


	


	

if(old_list != NULL){
	
				
				for(str_ptr = new_list->start ; str_ptr != NULL; str_ptr = str_ptr->next){
			
				if(DEBUG)
				printf("\nresult of strcmp = %d\n", strncmp((str_ptr->title), (old_list->start->title), (strlen(new_list->start->title)) ));

			
			
			
					if(strcmp(str_ptr->title, old_list->start->title) != 0){
						
						if(u8_print_flag != 0){
							
							if(str_ptr->title != NULL)
							printf("\nNew headline:\n%s\n\n", str_ptr->title);					
							if(str_ptr->link != NULL)			
							printf("\nLink: %s\n", str_ptr->link);
							if(str_ptr->description != NULL){
								printf("\nDescription: %s\n\n", str_ptr->description);
							}
					}
					#if 1
					exit(1);
					#endif
					}
					else{
						#if 0
						printf("\nupdate_service: Keine neue Nachricht gefunden\n");
						#endif
						/* for-loop verlassen */
						break;
					}
			
				}
				
	update_assistant(&delay_milliseconds, &u16_intervall);
	/* If old_list was not NULL */
	return 1;
}	
else{
	
	
	update_assistant(&delay_milliseconds, &delay_connection_lost);
return 0;		
}
/* if old_list was 0 */	
	

}



void update_assistant(const uint16_t *milliseconds, const uint16_t *seconds) {


	uint32_t u32_nano_seconds;
	struct timespec sleeptime, remainingtime;
	

	u32_nano_seconds = *milliseconds * 1000 * 1000;	
	sleeptime.tv_nsec = u32_nano_seconds;
	sleeptime.tv_sec = *seconds;

	nanosleep(&sleeptime, &remainingtime);


}


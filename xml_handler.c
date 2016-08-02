/*
 * xml_handler.c
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

#include <stdint.h>
#include "xml_handler.h"

#define DEBUG 0
#define RSS_END "</rss>"
#define RSS_START "<?xml"
#define MARKUP_START "(<"
#define READ 0

char *item_name = "item";
char *title_name = "title";
char *link_name = "link";
char *description_name = "description";
char *pub_date = "pubDate";

char *temp_title;
char *temp_description;
char *temp_link;
char *tmp_pub_date;

/* Zählt wieviele Nachrichten im xml-String waren */
uint16_t element_counter = 0;

int dom_parser(const char *content, int length,
	       struct_news_list * list_ptr)
{

	xmlDocPtr doc;		/* the resulting document tree */
	xmlNode *root_element = NULL;

	/*
	 * The document being in memory, it have no base per RFC 2396,
	 * and the "noname.xml" argument will serve as its base.
	 */
	doc =
	    xmlReadMemory(content, length, "noname.xml",
			  NULL, XML_PARSE_NOERROR /* 0 */ );

	if (doc == NULL) {
		fprintf(stderr,
			"Failed to parse document\n");
		return 1;
	}

	root_element = xmlDocGetRootElement(doc);

	element_counter = 0;

	get_dom_objects(root_element, list_ptr);

	/*free the document */
	xmlFreeDoc(doc);

	/*
	 *Free the global variables that may
	 *have been allocated by the parser.
	 */
	xmlCleanupParser();

	return 0;
}

void get_dom_objects(xmlNode * a_node,
		     struct_news_list * list_ptr)
{
	xmlNode *cur_node = NULL;

	uint16_t title_size, link_size, description_size,
	    item_size, date_size;

	title_size = strlen(title_name);
	link_size = strlen(link_name);
	description_size = strlen(description_name);
	item_size = strlen(item_name);
	date_size = strlen(pub_date);

	for (cur_node = a_node; cur_node;
	     cur_node = cur_node->next) {
		/* Typ: XML_ELEMENT_NODE */

		if (cur_node->type == XML_ELEMENT_NODE) {

			/* search for publish date */
			if (strncmp
			    ((char *)cur_node->name,
			     pub_date, date_size) == 0) {

				/* test if there is an cripple tag */
				if ((char *)cur_node->
				    children->content !=
				    NULL) {
					tmp_pub_date =
					    strdup((char *)
						   cur_node->
						   children->content);
				} else {
					tmp_pub_date = NULL;
				}
			}

			/* Wenn der Name ,,title" entspricht */

			if (strncmp
			    ((char *)cur_node->name,
			     title_name, title_size) == 0) {

				/* Wenn Parent = ,,item" entspricht */

				if (strncmp
				    ((char *)cur_node->
				     parent->name,
				     item_name,
				     item_size) == 0) {

					element_counter++;

					/* test if there is an cripple array */
					if (cur_node->
					    children !=
					    NULL) {
						if (READ)
							printf
							    ("Title [%d]: %s\n\n",
							     element_counter,
							     cur_node->children->
							     content);

						/* Es wird immer wieder die Startadresse übergeben */

						temp_title =
						    strdup((char *)
							   cur_node->children->content);

					} else {
						if (DEBUG)
							printf
							    ("\nTitle-array will be set to NULL");
						temp_title =
						    NULL;
					}
				}
			}

			/* Wenn der Name ,,link" entspricht */
			if (strncmp
			    ((char *)cur_node->name,
			     link_name, link_size) == 0) {

				/* Wenn Parent = ,,item" entspricht */

				if (strncmp
				    ((char *)cur_node->
				     parent->name,
				     item_name,
				     item_size) == 0) {

					/* test if there is a cripple xml tag */
					if (cur_node->
					    children !=
					    NULL) {

						if (READ)
							printf
							    ("Link [%d]: %s\n\n",
							     element_counter,
							     cur_node->children->
							     content);

						temp_link =
						    strdup((char *)
							   cur_node->children->content);
					} else {
						if (DEBUG)
							printf
							    ("\nLink-array will be set to NULL");
						temp_link =
						    NULL;
					}
				}

			}

			/* Wenn der Name ,,description" entspricht */
			if (strncmp
			    ((char *)cur_node->name,
			     description_name,
			     description_size) == 0) {

				/* Wenn Parent = ,,item" entspricht */

				if (strncmp
				    ((char *)cur_node->
				     parent->name,
				     item_name,
				     item_size) == 0) {
					char *clean_string,
					    *markup_start;
					int startzeichen,
					    string_lenght;
					startzeichen = 0;
					markup_start =
					    MARKUP_START;

					/* test if there is a cripple xml tag */
					if (cur_node->
					    children !=
					    NULL) {

						/* test if it is markup in the string */

						startzeichen
						    =
						    strstr((char *)
							   cur_node->children->content, markup_start)
						    -
						    (char *)
						    &cur_node->children->content;
						if (startzeichen > 0) {
							if (DEBUG)
								printf
								    ("\nStartzeichen gefunden bei: %d\n",
								     startzeichen);
							clean_string
							    =
							    malloc
							    ((startzeichen + 1) * sizeof(char)
							     /* valgrind */
							    );
							strncpy
							    (clean_string,
							     (char
							      *)
							     cur_node->children->content, startzeichen);

						} else {
							if (DEBUG)
								printf
								    ("\nStartzeichen ist null oder kleiner als null: %d\n",
								     startzeichen);

							string_lenght
							    =
							    strlen
							    ((char *)
							     cur_node->children->content);

							clean_string
							    =
							    malloc
							    ((string_lenght + 1) * sizeof(char)
							     /* valgrind */
							    );

							strncpy
							    (clean_string,
							     (char
							      *)
							     cur_node->children->content, string_lenght);
							clean_string
							    [string_lenght]
							    =
							    '\0';
						}

						/* strncpy(clean_string, cur_node->children->content, startzeichen); */
						if (READ) {
							printf
							    ("Description [%d]: %s\n\n\n\n",
							     element_counter,
							     clean_string);
						}
						temp_description
						    =
						    clean_string;

					} else {

						temp_description
						    = NULL;

					}

					append(&
					       (list_ptr->
						start),
					       &element_counter,
					       temp_title,
					       temp_link,
					       temp_description,
					       tmp_pub_date,
					       list_ptr);

				}

			}

			/* REKURSIVE FUNKTION! */
			get_dom_objects(cur_node->children,
					list_ptr);
			/* Ende for-Schleife */
		}
	}
}

struct_news_list *load_data(struct_adress * meta_info)
{

	int gesamtlaenge, counter2, startzeichen,
	    str_lenght;
	uint16_t u16_counter;
	char *end_tag = RSS_END;
	char *start_tag = RSS_START;
	char *temp;
	char *server_info;
	char *rss_string;
	struct_news_list *lists;

	lists = malloc(sizeof(*lists));

	lists->start = NULL;
	lists->end = NULL;

	if (DEBUG)
		printf("For-Schleife load_data\n");
	gesamtlaenge = strlen(meta_info->s_raw_string);

	if (DEBUG) {
		printf("\nLänge des String: %d\n",
		       gesamtlaenge);

		for (counter2 = 0; counter2 < 20;
		     counter2++) {
			printf("\nBuchtabe %d : ",
			       counter2);
			printf("%c",
			       meta_info->
			       s_raw_string[counter2]);
		}
	}

	/* Position des Startzeichens im array holen */
	startzeichen =
	    get_starttag(meta_info->s_raw_string,
			 start_tag);

	if ((startzeichen >= gesamtlaenge)
	    || (startzeichen <= 0)) {
		printf("\nServer query failed\n");
		exit(1);
	}

	if (DEBUG)
		printf
		    ("\nAnfangszeichen gefunden an Stelle %d\n",
		     startzeichen);

	/* Neue Länge des Strings */
	str_lenght = gesamtlaenge - startzeichen;

	/* temp enthält das xml-Dokument, server_info die Serverdaten die mitgesendet wurden */

	/* temp = malloc(str_lenght * sizeof(char)); valgrind */
	server_info = malloc(startzeichen + 1);

	if (DEBUG)
		printf("\nNeue Länge des Strings: %d\n",
		       str_lenght);

	/* String temp mit XML-Daten erzeugen */

	temp =
	    get_temp_string(meta_info->s_raw_string,
			    startzeichen, str_lenght);

	/* String server_info mit den am Anfang gesendeten Serverdaten erzeugen */

	/* rest vom temp String entfernen */

	rss_string =
	    get_rss_tag(temp, end_tag, &str_lenght);

	free(temp);
	free(server_info);

#if 1
	if (DEBUG) {
		printf(" \nFor-Schleife load_data\n");
		for (u16_counter = (strlen(rss_string) - 1);
		     u16_counter > strlen(rss_string) - 20;
		     u16_counter--) {
			printf("\nBuchtabe %d : ",
			       u16_counter);
			printf("%c",
			       rss_string[u16_counter]);
		}

		printf
		    ("\nInteger load_data strlen rss_lenght: %d\n",
		     (int)strlen(rss_string));
		printf("\nZeichen bei str_lenght %d : %c\n",
		       (int)strlen(rss_string),
		       rss_string[(int)strlen(rss_string)]);
		printf("\nZeichen bei position %d : %c\n",
		       (int)(strlen(rss_string) - 1),
		       rss_string[(strlen(rss_string) -
				   1)]);
	}
#endif
	if (meta_info->b_dom_parser == true) {
		if ((dom_parser
		     (rss_string, (strlen(rss_string)),
		      lists)) == 0) {

			free(rss_string);

			if (DEBUG)
				printf
				    ("\nexamplefunc succeed\n");

			return lists;
		} else {
			free(rss_string);
			free(lists);
			if (DEBUG)
				printf
				    ("\nexamplefunc failed\n");
			return NULL;
		}
	} else {
		printf
		    ("\nThe DOM-Parser is currently the only working parser, sorry!\n");
		exit(1);
	}
}

char *get_temp_string(char *xml_string, int startzeichen,
		      int str_lenght)
{
	/* str_lenght = neue länge */
	char *temp2, *temp3;

	/* str_lenght = overall lenght - start-sign */

	temp2 = malloc((str_lenght + 1) * sizeof(char));
	temp3 =
	    xml_string +
	    (size_t) ((startzeichen) / sizeof(char));

	strncpy(temp2, temp3, (str_lenght + 1));
	return temp2;
}

char *get_server_info(char *xml_string, int startzeichen)
{

	char *server_info;
	server_info = (char *)malloc((startzeichen + 1) * sizeof(char));	/* valgrind */

	strncpy(server_info, xml_string, startzeichen + 1);
	server_info[startzeichen] = '\0';
	return server_info;
}

char *get_rss_tag(char *temp_string, char *end_tag,
		  const int *str_lenght)
{
	uint16_t counter, tag_lenght, rss_lenght;
	char *check_tag;
	char *rss_string;

	tag_lenght = strlen(end_tag);
	check_tag = malloc(tag_lenght);

	if (DEBUG) {
		printf("\nInteger str_lenght: %d\n",
		       *str_lenght);
		printf("\nInteger tag_lenght: %d\n",
		       tag_lenght);
	}

	for (counter = *str_lenght; counter > 0; counter--) {

		strncpy(check_tag,
			temp_string + (size_t) (counter -
						tag_lenght),
			tag_lenght);

		/* Vergleichen ob end_tag und check_tag übereinstimmen */
		if (strncmp(check_tag, end_tag, tag_lenght)
		    == 0) {
			rss_lenght = counter;
			if (DEBUG)
				printf
				    ("\nEnde gefunden bei zeichen Nummer: %d\n",
				     counter);
			break;
		}

	}
	free(check_tag);
	/* endgültiger String wird erstellt, Ende (unbrauchbar) wird entfernt */
	rss_string =
	    malloc((rss_lenght + 1) * sizeof(char));
	memset(rss_string, 0, sizeof(rss_lenght + 1));
	strncpy(rss_string, temp_string, rss_lenght);

	/* Das letzte Zeichen muss manuel auf \0 gesetzt werden */
	rss_string[rss_lenght] = '\0';
	if (DEBUG)
		printf("\nInteger rss_lenght: %d\n",
		       rss_lenght);

	return rss_string;
}

int get_starttag(const char *xml_string,
		 const char *start_tag)
{

	size_t startzeichen;

	startzeichen =
	    (size_t) strstr(xml_string,
			    start_tag) -
	    (size_t) xml_string;
	if (DEBUG)
		printf("\nStartzeichen gefunden bei: %lu\n",
		       startzeichen);
	return startzeichen;
}

void
append(struct_news ** lst, uint16_t * position,
       char *title, char *link, char *description,
       char *tmp_pub_date, struct_news_list * list_ptr)
{

	struct_news *ref_ptr, *new_element;

	if (DEBUG) {
		printf
		    ("\nappend (nach load_data): Adresse auf die *lst zeigt = %lu\n",
		     (long unsigned int)*lst);
		printf
		    ("\nappend (nach load_data): Adresse von &(*lst) selbst= %lu\n",
		     (long unsigned int)&(*lst));
		printf
		    ("\nappend (nach load_data): &lst : lst = %lu\n",
		     (long unsigned int)&lst);

		/* Das hier ist wieder ein ganz normaler pointer */
		ref_ptr = *lst;	/* 1x dereferenziert = Adresse des Pointers */
		/* 2x dereferenziert = Adresse des struct */
		/* 0x dereferenziert = Adresse des Pointers auf die dieser Pointer zeigt */
		printf
		    ("\nappend (nach load_data): Adresse auf die ref_ptr zeigt= %lu\n",
		     (long unsigned int)ref_ptr);
		printf
		    ("\nappend (nach load_data): Adresse von ref_ptr selbst %lu\n",
		     (long unsigned int)&ref_ptr);
		/* funktionier nicht */
		/* printf("\nappend (vor load_data): Inhalt vom Struct auf das new_element zeigt %x\n", *new_element); */
	}

	new_element = malloc(sizeof(*new_element));

	/* Während die Adresse von *lst (new_element) != Null ist */
	/* Bei dem ersten durchlauf wird die while-Schleife übersprungen */

	/* lst wird durchgeschaltet und der Inhalt geprüft */

	new_element->previous = NULL;

	while (*lst != NULL) {

		/* *lst = ursprünglicher pointer */
		/* lst = Inhalt des Struct */
		/* *lst = Adresse des struct element */
		/* *lst bekommt die Adresse auf das nächste Element */

		/* Ausprobieren */
		/* müsste beim ersten Element NULL sein */
		new_element->previous = *lst;

		/* Erst wenn *lst = NULL ist, wird die while-Schleife verlassen */
		/* lst(Adresse des Struct-Pointers) = (Adresse des Struct Element)->(Ponter auf nächstes Element) */
		/* Inhalt des Zielstruct geht hier nur weil != NULL */

		lst = &((*lst)->next);

		/* wenn *lst auf NULL zeigt(kein Inhalt vorhanden), zeigt lst auch auf das nächste Element (NULL) und die while-schleife wird verlassen */
		/* Nullpointer! */
	}
	if (DEBUG)
		printf
		    ("\nappend(): adress of struct: %lu, position: %d\n",
		     (long unsigned int)*lst, *position);

	/* Speicher reservieren: Pointer soll auf einen freien Bereich im Speicher zeigen */

	new_element->position = *position;

	new_element->title = title;
	new_element->link = link;
	new_element->description = description;
	new_element->pub_date = tmp_pub_date;

	new_element->next = NULL;	/* Das aktuelle Element ist das Ende der Liste */

	/* *lst = Adresse = new_element */

	/* Hier stimmt was nicht */

	*lst = new_element;

	list_ptr->end = new_element;

	if (DEBUG)
		printf("\nappend(): Leaving append()n");
}

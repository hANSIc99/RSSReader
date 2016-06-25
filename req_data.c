/*
 * req_data.c
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


#include "req_data.h"
#define DEBUG 0

#define REQ1 "GET /"
#define REQ2 " HTTP/1.1\nHOST: "
#define REQ3 "\n\n\n"

/* 
           struct addrinfo {
               int              ai_flags;
               int              ai_family;
               int              ai_socktype;
               int              ai_protocol;
               socklen_t        ai_addrlen;
               struct sockaddr *ai_addr;
               char            *ai_canonname;
               struct addrinfo *ai_next;
           };
*/
/*
    struct sockaddr {
    unsigned short    sa_family;    // address family, AF_xxx
    char              sa_data[14];  // 14 bytes of protocol address
};
*/


/* Funktion, die einen Pointer zurückgibt */
/* Pointer auf sockaddr_in oder sockaddr_in6 */


char buf[MAXDATASIZE];

void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
		/* Cast: sockaddr zu sockaddr_in(6) ) */
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

char *req_server(const struct_adress *rss_server)
{
	if(rss_server != NULL){
	
	
	struct addrinfo hints,*res, *p;
	int addr_status, sock_status, connect_status, numbytes, send_status;
	char ipstr[INET6_ADDRSTRLEN];
	struct sockaddr_in *port;
	char *req;
	char *req1 = REQ1;
	char *req2 = REQ2;
	char *req3 = REQ3;
	uint16_t u16_str_lenght = 0;
	


	/* Ther terminating  \0 character must be added to the size in [byte] */
	u16_str_lenght = strlen(req1) + strlen(req2) + strlen(req3) + strlen(rss_server->s_domain) + strlen(rss_server->s_request) +1;
	#if 0
	printf("\n string gesamt lenght: %d\n", u16_str_lenght);
	printf("\n 1 lenght: %d\n", strlen(REQ1));
	printf("\n 2 lenght: %d\n", strlen(REQ2));
	printf("\n 3 lenght: %d\n", strlen(REQ3));
	printf("\n 4 lenght: %d\n", strlen(rss_server->s_domain));
	printf("\n 5 lenght: %d\n", strlen(rss_server->s_request));
	#endif
	req = (char*)malloc(u16_str_lenght * sizeof(char));
	
	strcpy(req, req1);
	strcat(req, rss_server->s_request);
	strcat(req, req2);
	strcat(req, rss_server->s_domain);
	strcat(req, req3);

	
	#if 0
	printf("\n\nkompletter String: %s\n", req);
	#endif
	/* char req1[] = "GET /rss.php?feed=RSS2.0 HTTP/1.1\nHOST: rss.golem.de\n\n\n"; */
	/* char req1[] = "GET /newsticker/heise.rdf HTTP/1.1\nHOST: www.heise.de\n\n\n"; */
	/* char req1[] = "GET /fol/XML/rss_folnews.xml HTTP/1.1\nHOST: rss.focus.de\n\n\n"; */
	/* char req1[] = "GET /schlagzeilen/tops/index.rss HTTP/1.1\nHOST: www.spiegel.de\n\n\n"; */
	/* char req1[] = "GET /rss/cnn_topstories.rss HTTP/1.1\nHOST: edition.cnn.com\n\n\n"; */
	/* char req1[] = "GET /" + rss_server->s_request + "HTTP/1.1\nHOST: " + rss_server->s_domain + "\n\n\n"; */
	
	#if 0
	if(argc != 2){
		if(argc < 2)
		fprintf(stderr, "ERROR: ZU WENIGE PARAMETER; BENÖTIGT: DOMÄNE ODER IP\n");
		else
		fprintf(stderr, "ERROR: ZU VIELE PARAMETER; BENÖTIGT: DOMÄNE ODER IP\n");
		return NULL;
	}
	#endif
	/* stdio.h : speicher beschreiben; hier: Speicher nullen */
	memset(&hints, 0, sizeof hints);
	
	/* Vorgabewerte setzen */
	
	hints.ai_family = AF_UNSPEC;		/* AF_INET oder AF_INET6 falls notwenig */
	hints.ai_socktype = SOCK_STREAM;	/* TCP */

	
	/* 
int getaddrinfo(const char* hostname,
                const char* service,
                const struct addrinfo* hints,
                struct addrinfo** res);
                
##hostname can be either a domain name, such as "example.com", an address string, such as "127.0.0.1", or NULL, in which case the address 0.0.0.0 or 127.0.0.1 is assigned depending on the hints flags.
##service can be a port number passed as string, such as "80", or a service name, e.g. "echo". In the latter case, gethostbyname() is used to query the file /etc/services to resolve the service to a port number.
##hints can be either NULL or an addrinfo structure with the type of service requested.
##res is a pointer that points to a new addrinfo structure with the information requested after successful completion of the function. 
                
    */
	
	if((addr_status = getaddrinfo( rss_server->s_domain /* "www.themoscowtimes.com" */ /* "edition.cnn.com" */ /*  "www.spiegel.de"  "rss.focus.de"  "rss.golem.de"   "www.heise.de" */, PORT, &hints, &res)) != 0) {	/* 0 bei erfolg, -1 bei Fehler */
		fprintf(stderr, "ERROR: getaddrinfo(): %s\n", gai_strerror(addr_status));
		return NULL;
	}
	
	/* return NULL; */
	for(p = res; p != NULL; p = p->ai_next){	/* Durlaufen der verketteten Liste */


		
	 /* Abfrage welche IP-Version in addrinfo verwendet wird */
	 	#if 0
		
		if(p->ai_family == AF_INET){	/* IPv4 */
			
			/*
			struct sockaddr_in
			{
				short   sin_family; 
				u_short sin_port;
				struct  in_addr sin_addr;
				char    sin_zero[8]; 
			};
			*/
			/*
			struct in_addr {
				uint32_t s_addr; // that's a 32-bit int (4 bytes)
			};
			*/
			
			struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;	
			addr = &(ipv4->sin_addr);	/* addr zeigt jetzt auf die IPv4 Adresse des Servers */	 
										
			ipver = "IPv4";
			printf("\nIPv4: Rohdaten vor inet_ntop(): p->ai_family: %d,  ipv4->sin_addr.s_addr: %d\n", p->ai_family, ipv4->sin_addr.s_addr);
			printf("\nIPv4: Port-Nummer: %d\n", ipv4->sin_port);
			

		 
		}
		else {	/* IPv6 */
			struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
			addr = &(ipv6->sin6_addr);
			ipver = "IPv6";
			printf("\nIPv6: Rohdaten vor inet_ntop(): p->ai_family: %d,  ipv6->sin6_addr.s6_addr: ", p->ai_family);
			
			int i;
			printf("\n");
			for(i = 0; i<16; i++){
				printf("Char %u bei i = %d\n", ipv6->sin6_addr.s6_addr[i], i);
			}
		}
		
		/* Umwandeln der IP-Adresse in String */
		/* ntop = "network to printable*/
		
		/* const char *inet_ntop(int af, const void *src, char *dst, socklen_t size); */
		
		/* af = adress family; AF_INET oder AF_INET6 
		 * *src = pointer zu einem in_addr/in_addr6 struct der die zu konvertierende Adresse enthält 
		 * *dst = Ziel-String
		 * size = maximale Länge des String */
		 
		inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr);
		
		printf("\n %s: %s\n", ipver, ipstr);
		
		#endif
		
			/*
	 * int socket(int domain, int type, int protocol); 
	 * 
	 * domain = PF_INET oder PF_INET6
	 * -> es kann aber auch das feld ai_family aus dem struct addrinfo verwendet werden!
	 * dann AF_INET oder AF_INET6
	 * 
	 * type = SOCK_STREAM oder SOCK_DGRAM
	 * 
	 * protocol = can be set to 0 to choose the proper protocol for the given type
	 * Oder es kann getprotobyname() verwendet werden, um nach den entsprechenden
	 * Protokoll zu suchen (bsp.: "tcp" oder "udp")
	 */
	if(DEBUG) 
	printf("\nParameter für socket(): %d, %d, %d\n", res->ai_family, res->ai_socktype, res->ai_protocol);
	
	/* Es wird ein socket erstellt */
	
	if((sock_status = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1){
		fprintf(stderr, "\nERROR: socket() FEHLGESCHLAGEN\n");
		continue;
	}
	else {
		if(DEBUG)
		printf("\nsocket(): sock_status = %d\n", sock_status);
	}
	
	/* int connect (int sockfd, struct sockaddr* serv_addr, int addrlen);
	 * 
	 * sockfd = socked-file-descriptor
	 * 
	 * serv_addr = struct sockaddr, benötigt IP und Port
	 * 
	 * addrlen = Länge der Adresse in Bytes
	 */
	 
	port = (struct sockaddr_in *)p->ai_addr;
	if(DEBUG)
	printf("\nParameter für connect(): %d, Port-Nummer: %d, %d\n", sock_status, htons(port->sin_port), p->ai_addrlen);
	 

	 
	if((connect_status = connect(sock_status, p->ai_addr, p->ai_addrlen)) == -1){
		fprintf(stderr, "ERROR: connect() FEHLGESCHLAGEN\n");
		close(sock_status);
		printf("\nconnect_status = %d\n", connect_status);
		continue;
	}
		else {
			if(DEBUG)
		printf("\nconnect(): connect_status = %d -> Verbindung hergestellt! \n", connect_status);
	}
	
break;
	}
	
	
	if(p == NULL){
		fprintf(stderr, "\nERROR: KEINE VERBINDUNG MÖGLICH\n");
		return NULL;
	}
		
	/* IP-Adresse umwandeln in String */
		
	inet_ntop(p->ai_family, get_in_addr(p->ai_addr), ipstr, sizeof ipstr);
	
	
	if(DEBUG)
	printf("\nConnecting to: %s\n", ipstr);
	

			
	/* res wurde mit alloc initialisiert und kann jetzt gelöscht werden  */
	freeaddrinfo(res);	 

		 
	 	 if((send_status = send(sock_status, req, u16_str_lenght, 0)) != (u16_str_lenght)){
		
		 fprintf(stderr, "\nERROR: send() #1 FEHLGESCHLAGEN: Nachrichtenlänge: %d, gesendet %d\n", (u16_str_lenght+1), send_status);
		
		}
		



	free(req);
	
	
	
	
	
	
	
	
	
	/* int recv(int sockfd, void *buf, int len, int flags);
	 * 
	 * sockfd = socked file descriptor
	 * 
	 * *buf = nimmt die gelesenen Daten auf
	 * 
	 * len = maximalgröße des buffer
	 * 
	 * flags können auf 0 gesetzt werden
	 */
	
	if((numbytes = recv(sock_status, buf, MAXDATASIZE-1, MSG_WAITALL)) == -1){
		fprintf(stderr, "\nERROR: recv() FEHLGESCHLAGEN\n");
        return NULL;
	}
	
	buf[numbytes] = '\0' ;
	
	if(DEBUG){
	printf("\nMaximale Puffergröße = %d Bytes\n", MAXDATASIZE);
	printf("\nEmpfangende Bytes: %d\n", numbytes);
	printf("\nEmpfangende Bytes: %d\n", numbytes);
	printf("\n DATEN: %s\n", buf);
	}
	close(sock_status);
	
	

	return buf;
}
else{
	return NULL;
}
}


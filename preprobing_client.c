#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "client.h"

void pre_probe(char* buffer, struct configurations *configs) {
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == -1) {
	    perror("Socket creation failed");
	    exit(EXIT_FAILURE);
	}

	struct sockaddr_in sin;
	memset(&sin, 0, sizeof(sin));

	in_addr_t server_addr = inet_addr(configs->server_ip_addr);

	sin.sin_family = AF_INET; /* address from Internet, IP address specifically */
	sin.sin_addr.s_addr = server_addr; /* already in network order */
	sin.sin_port = htons(configs->server_port_preprobing); /* convert to network order */

	if (connect(sock, (struct sockaddr *) &sin, sizeof(sin)) == -1) {
		perror("Cannot connect to server");
		close(sock);
		exit(EXIT_FAILURE);
	}
	
	int count = send(sock, buffer, strlen(buffer), 0);

	if (count == -1) {
		perror("Failed to send configurations");
		close(sock);
		exit(EXIT_FAILURE);
	}

	close(sock);
}

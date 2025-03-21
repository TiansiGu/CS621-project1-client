#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "client.h"

#define BUF_SIZE 64

void post_probe(struct configurations *configs) {
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
	sin.sin_port = htons(configs->server_port_postprobing); /* convert to network order */

	if (connect(sock, (struct sockaddr *) &sin, sizeof(sin)) == -1) {
		perror("Cannot connect to server");
		close(sock);
		exit(EXIT_FAILURE);
	}

	char buffer[BUF_SIZE];
	fd_set read_fds;
	// Initialize the file descriptor set to zero
	FD_ZERO(&read_fds);
	// Add the socket to the file descriptor set
	FD_SET(sock, &read_fds);
	while (1) {
		int res = select(sock + 1, &read_fds, NULL, NULL, NULL);
		if (res != -1) {
			int count = recv(sock, buffer, BUF_SIZE, 0);
			if (count == -1) {
				perror("Failed to receive detection result from server");
				close(sock);
				exit(EXIT_FAILURE);
			}
			buffer[count] = '\0';
			printf("%s\n", buffer); //print detection result in the console
			break;
		}
	}
}

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>

#include "client.h"

void generate_random_bytes(unsigned char *ptr, int size) {
	int randomData = open("/dev/urandom", O_RDONLY);
	if (randomData < 0) {
		perror("Cannot open /dev/urandom");
		exit(EXIT_FAILURE);
	}

	/** "/dev/urandom" can return fewer bytes than you've asked for when there is not 
	enough bytes. Solution: Keep reading until the requested size is fully received. */
	size_t randomDataLen = 0;
	while (randomDataLen < size) {
		ssize_t read_bytes = read(randomData, ptr + randomDataLen, size - randomDataLen);
		if (read_bytes < 0) {
			perror("Failed to read in random bytes");
			close(randomData);
			exit(EXIT_FAILURE);
		}
		randomDataLen += read_bytes;
	}

	close(randomData);
}

unsigned char * creat_bytes(int size, int entropy_high) {
	unsigned char *data_ptr = malloc(size);
	if (data_ptr == NULL) {
		perror("Failed to allocate memory for UDP packet data");
		exit(EXIT_FAILURE);
	}
	if (entropy_high) {
		generate_random_bytes(data_ptr, size);
	} else {
		memset(data_ptr, 0, size);
	}
	return data_ptr;
}

void probe(struct configurations *configs) {
	unsigned char *low_entropy_data = creat_bytes(configs->l, 0);
	unsigned char *high_entropy_data = creat_bytes(configs->l, 1);

    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == -1) {
	    perror("Socket creation failed");
	    exit(EXIT_FAILURE);
	}


	struct sockaddr_in sin;
	memset(&sin, 0, sizeof(sin));

	in_addr_t server_addr = inet_addr(configs->server_ip_addr);

    sin.sin_family = AF_INET; /* address from Internet, IP address specifically */
	sin.sin_addr.s_addr = server_addr; /* already in network order */
	sin.sin_port = htons(configs->udp_dst_port); /* convert to network order */

	int count;
	
	// char ready_buff[32];
	// count = recvfrom(sock, ready_buff, sizeof(ready_buff), 0, NULL, NULL);
	// if (count == -1) {
	// 	perror("Failed to receive ready signal");
	// 	close(sock);
	// 	exit(EXIT_FAILURE);
	// }
	// ready_buff[count] = '\0';
    // if (strcmp(ready_buff, "READY") != 0) {
	// 	perror("Server did not give expected response");
	// 	close(sock);
	// 	exit(EXIT_FAILURE);
	// } 

	char *buffer = "hello I am UDP";
	count = sendto(sock, buffer, 32, 0, (struct sockaddr *) &sin, sizeof(sin));
	
	if (count == -1) {
		perror("Failed to send UDP packet");
		close(sock);
		exit(EXIT_FAILURE);
	}
	
	close(sock);
}

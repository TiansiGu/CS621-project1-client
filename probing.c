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

unsigned char * generate_bytes(int size, int entropy_high) {
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

void bind_port(int fd, int port, struct sockaddr_in *addr) {
	addr->sin_family = AF_INET;
    addr->sin_addr.s_addr = INADDR_ANY;
    addr->sin_port = htons(port);

	if (bind(fd, (struct sockaddr*) addr, sizeof(struct sockaddr_in)) == -1) {
		perror("Failed to bind socket");
		close(fd);
		exit(EXIT_FAILURE);
	}
}

void probe(struct configurations *configs) {
	unsigned char *low_entropy_data = generate_bytes(configs->l, 0);
	unsigned char *high_entropy_data = generate_bytes(configs->l, 1);

    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == -1) {
	    perror("Socket creation failed");
	    exit(EXIT_FAILURE);
	}

	struct sockaddr_in client_sin, server_sin;
	memset(&client_sin, 0, sizeof(client_sin));
	memset(&server_sin, 0, sizeof(server_sin));

	in_addr_t server_addr = inet_addr(configs->server_ip_addr);

    server_sin.sin_family = AF_INET; /* address from Internet, IP address specifically */
	server_sin.sin_addr.s_addr = server_addr; /* already in network order */
	server_sin.sin_port = htons(configs->udp_dst_port); /* convert to network order */

	// Specify the socket client uses to connect to server
	bind_port(sock, configs->udp_src_port, &client_sin);

	int count;

	// Send low entropy packet train
	for (int i = 0; i < configs->n; i++) {
		count = sendto(sock, low_entropy_data, configs->l, 0, (struct sockaddr *) &server_sin, sizeof(server_sin));
		if (count == -1) {
			perror("Failed to send UDP packets with low entropy data");
			close(sock);
			exit(EXIT_FAILURE);
		}
	}

	// Wait γ secs before sending the high entropy packet train
	sleep(configs->gamma);
	
	// Send high entropy packet train
	for (int i = 0; i < configs->n; i++) {
		count = sendto(sock, high_entropy_data, configs->l, 0, (struct sockaddr *) &server_sin, sizeof(server_sin));
		if (count == -1) {
			perror("Failed to send UDP packets with high entropy data");
			close(sock);
			exit(EXIT_FAILURE);
		}
	}
	
	close(sock);
}

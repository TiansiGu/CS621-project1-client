#include <stdint.h>
#define ADDR_LEN 32

struct configurations {
	char server_ip_addr[ADDR_LEN];
	uint16_t server_port_preprobing;
	uint16_t udp_src_port;
	uint16_t udp_dst_port;
	uint16_t l; // the Size of the UDP Payload in a UDP Packet
};

void pre_probe(char *, struct configurations *);

void probe(struct configurations *);

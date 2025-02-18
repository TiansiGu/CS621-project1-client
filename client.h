#define ADDR_LEN 32

struct configurations {
	char server_ip_addr[ADDR_LEN];
	uint16_t server_port_preprobing;
	uint16_t udp_src_port;
	uint16_t udp_dst_port;
	
};

void pre_probe(struct configurations *);

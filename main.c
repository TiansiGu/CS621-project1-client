#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cjson/cJSON.h> 

#define ADDR_LEN 20

struct configurations {
	char server_ip_addr[ADDR_LEN];
	int udp_src_port;
	int udp_dst_port;
	
};

void parse_configs(char* file_name, struct configurations *configs) {
	// open the json file
	FILE *fp = fopen(file_name, "r");
	if (fp == NULL) {
		printf("Error: Unable to open the file.\n"); 
	    exit(1);
	}
	// read the file contents into a string 
	char buffer[1024]; 
	int len = fread(buffer, 1, sizeof(buffer), fp);
	fclose(fp);

	// parse the JSON data 
	cJSON *json = cJSON_Parse(buffer); 
	if (json == NULL) { 
		const char *error_ptr = cJSON_GetErrorPtr(); 
		if (error_ptr != NULL) { 
	    	 printf("Error: %s\n", error_ptr); 
	    } 
	    cJSON_Delete(json); 
	    exit(1); 
	}
	
	// access the JSON data 
	cJSON *name = cJSON_GetObjectItemCaseSensitive(json, "server_ip_addr"); 
	if (cJSON_IsString(name) && (name->valuestring != NULL)) { 
	    strcpy(configs->server_ip_addr, name->valuestring);
	}
	name = cJSON_GetObjectItemCaseSensitive(json,"udp_src_port"); 
	if (cJSON_IsNumber(name)) { 
		configs->udp_src_port = name->valueint;
	}

	name = cJSON_GetObjectItemCaseSensitive(json,"udp_dst_port"); 
	if (cJSON_IsNumber(name)) { 
		configs->udp_dst_port = name->valueint;
	}
	  
	// delete the JSON object 
	cJSON_Delete(json);  
}


int main(int argc, char* argv[]) {
	struct configurations configs;
	memset(&configs, 0, sizeof(struct configurations));
	char* file_name = argv[1];
	parse_configs(file_name, &configs);

	
	return 0;
}

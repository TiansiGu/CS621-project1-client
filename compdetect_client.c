#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cjson/cJSON.h>
#include "client.h" 

#define BUFFER_SIZE 1024

void parse_configs(char* file_name, char *buffer, struct configurations *configs) {
	// open the json file
	FILE *fp = fopen(file_name, "r");
	if (fp == NULL) {
		perror("Unable to open configuration file"); 
		exit(1);
	}
	// read the file contents into a string 
	int len = fread(buffer, 1, BUFFER_SIZE, fp);
	buffer[len] = '\0';
	fclose(fp);

	// parse the JSON data 
	cJSON *json = cJSON_Parse(buffer); 
	if (json == NULL) { 
		const char *error_ptr = cJSON_GetErrorPtr(); 
		if (error_ptr != NULL) { 
	    	 printf("Error when parsing json str: %s\n", error_ptr); 
	    } 
	    cJSON_Delete(json); 
	    exit(1); 
	}
	
	// access the JSON data 
	cJSON *name = cJSON_GetObjectItemCaseSensitive(json, "server_ip_addr"); 
	if (cJSON_IsString(name) && (name->valuestring != NULL)) { 
	    strcpy(configs->server_ip_addr, name->valuestring);
	}
	
	name = cJSON_GetObjectItemCaseSensitive(json,"server_port_preprobing"); 
	if (cJSON_IsNumber(name)) { 
		configs->server_port_preprobing = name->valueint;
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
	char buffer[BUFFER_SIZE];
	char* file_name = argv[1];
	parse_configs(file_name, buffer, &configs);
	pre_probe(buffer, &configs);
	
	return 0;
}

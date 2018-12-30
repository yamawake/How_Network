#include <stdio.h>
#include <stdlib.h>	// free()
#include <string.h>	// strdup(), strtok_r(), strtol()
#include <sys/types.h>
#include <netinet/in.h>
#include "ether.h"
#include "param.h"


extern PARAM	Param;

char *my_ether_ntoa_r(u_int8_t *hwaddr, char *buf){
	
	sprintf(buf, "%02x:%02x:%02x:%02x:%02x:%02x",
			hwaddr[0], hwaddr[1], hwaddr[2], hwaddr[3], hwaddr[4], hwaddr[5]);

	return(buf);
}


int my_ether_aton(char *str, u_int8_t *mac){

	char	*ptr;
	char	*saveptr=NULL;
	char	*tmp=strdup(str);
	int	c;

	for(c=0, ptr=strtok_r(tmp, ":", &saveptr); c < 6; c++ , ptr=strtok_r(NULL, ":", &saveptr)){
		if(ptr==NULL){
			free(tmp);
			return(-1);
		}
		mac[c]=strtol(ptr, NULL, 16);
	}
	free(tmp);

	return(0);
}

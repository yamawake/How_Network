#include <stdio.h>
#include <stdlib.h>	// free()
#include <string.h>	// strdup(), strtok_r(), strtol()
#include <sys/types.h>
#include <netinet/in.h>
#include <net/ethernet.h>	// ether_header
#include "ether.h"
#include "param.h"
#include "arp.h"
#include "ip.h"


extern PARAM	Param;

u_int8_t	BcastMac[6]={0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

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


int EtherRecv(int soc, u_int8_t *in_ptr, int in_len){

	struct ether_header	*eh;
	u_int8_t	*ptr=in_ptr;
	int	len=in_len;


	eh=(struct ether_header *)ptr;
	ptr+=sizeof(struct ether_header);
	len-=sizeof(struct ether_header);

	if(memcmp(eh->ether_dhost, BcastMac, 6)!=0&&memcmp(eh->ether_dhost, Param.vmac, 6)!=0){
		return(-1);
	}
	if(ntohs(eh->ether_type)==ETHERTYPE_ARP){
		ArpRecv(soc, eh, ptr, len);
	}
	else if(ntohs(eh->ether_type)==ETHERTYPE_IP){
		IpRecv(soc, in_ptr, in_len, eh, ptr, len);
	}

	return(0);
}

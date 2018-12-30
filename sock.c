#include <stdio.h>
#include <string.h>	// strcpy()
#include <sys/types.h>	// u_int8_t
#include <sys/socket.h>	// socket()
#include <sys/ioctl.h>	// ioctl()
#include <linux/if.h>	// ifreq
#include <linux/if_ether.h>	// ETH_P_ALL
#include <unistd.h>	// close()
#include <arpa/inet.h>	// htons()
#include <netpacket/packet.h>	// sockaddr_ll
#include "sock.h"

int GetMacAddress(char *device, u_int8_t *hwaddr)
{
	int soc;
	struct ifreq	ifreq;
	u_int8_t	*p;


	if((soc=socket(AF_INET, SOCK_DGRAM, 0))<0){
		perror("GetMacAddress():socket");
		return(-1);
	}

	strncpy(ifreq.ifr_name, device, sizeof(ifreq.ifr_name)-1);
	if(ioctl(soc, SIOCGIFHWADDR, &ifreq)==-1){
		perror("GetMacAddress():ioctl:hwaddr");
		close(soc);
		return(-1);
	}
	else{
		p=(u_int8_t *)&ifreq.ifr_hwaddr.sa_data;
		memcpy(hwaddr, p, 6);
		close(soc);
		return(1);
	}
}

int init_socket(char *device)
{
	int soc;
	struct ifreq	if_req;
	struct sockaddr_ll	sa;

	if((soc=socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL)))<0){
		perror("socket");
		return(-1);
	}

	strcpy(if_req.ifr_name, device);
	if(ioctl(soc, SIOCGIFINDEX, &if_req)<0){
		perror("ioctl");
		close(soc);
		return(-1);
	}

	sa.sll_family=PF_PACKET;
	sa.sll_protocol=htons(ETH_P_ALL);
	sa.sll_ifindex=if_req.ifr_ifindex;
	if(bind(soc,(struct sockaddr *)&sa, sizeof(sa))<0){
		perror("bind");
		close(soc);
		return(-1);
	}

	if(ioctl(soc, SIOCGIFFLAGS, &if_req)<0){
		perror("ioctl");
		close(soc);
		return(-1);
	}

	if_req.ifr_flags=if_req.ifr_flags|IFF_PROMISC|IFF_UP;
	if(ioctl(soc, SIOCSIFFLAGS, &if_req)<0){
		perror("ioctl");
		close(soc);
		return(-1);
	}

	return(soc);
}

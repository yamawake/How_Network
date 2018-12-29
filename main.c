#include <stdio.h>
#include <unistd.h>	// close()
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ioctl.h>	// ioctl()
#include <netinet/in.h>
#include <linux/if.h>
#include <arpa/inet.h>
#include "ether.h"
#include "param.h"
#include "sock.h"

PARAM	Param;


int show_ifreq(char *name)
{
	int soc;
	struct ifreq	ifreq;	// must include <linux/if.h>
	struct sockaddr_in	addr;
	char buf1[80];


	if((soc=socket(AF_INET, SOCK_DGRAM, 0))==-1){
		perror("socket");
		return(-1);
	}

	strcpy(ifreq.ifr_name, name);

	if(ioctl(soc, SIOCGIFFLAGS, &ifreq)==-1){
		perror("ioctl:flags");
		close(soc);
		return(-1);
	}

	if(ifreq.ifr_flags&IFF_UP){printf("UP ");}
	if(ifreq.ifr_flags&IFF_BROADCAST){printf("BROADCAST ");}
	if(ifreq.ifr_flags&IFF_PROMISC){printf("PROMISC ");}
	if(ifreq.ifr_flags&IFF_MULTICAST){printf("MULTICAT ");}
	if(ifreq.ifr_flags&IFF_LOOPBACK){printf("LOOPBACK ");}
	if(ifreq.ifr_flags&IFF_POINTOPOINT){printf("P2P ");}
	printf("\n");

	if(ioctl(soc, SIOCGIFMTU, &ifreq)==-1){
		perror("ioctl:mtu");
	}
	else{
		printf("mtu=%d\n", ifreq.ifr_mtu);
	}

	if(ioctl(soc, SIOCGIFADDR, &ifreq)==-1){
		perror("ioctl:addr");
	}
	else if(ifreq.ifr_addr.sa_family!=AF_INET){
		printf("not AF_INET\n");
	}
	else{
		memcpy(&addr, &ifreq.ifr_addr, sizeof(struct sockaddr_in));
		printf("myip=%s\n", inet_ntop(AF_INET,&addr.sin_addr, buf1, sizeof(buf1)));	// must include <arpa/inet.h>
		Param.myip=addr.sin_addr;
	}

	close(soc);

	if(GetMacAddress(name, Param.mymac)==-1){
		printf("GetMacAddress:error");
	}
	else{
		printf("mymac=%s\n", my_ether_ntoa_r(Param.mymac, buf1));

	}



	return(0);

}

int main(int argc, char *argv[])
{
	int paramFlag=0;
	char buf1[80];

	SetDefaultParam();

	for (int i = 1; i < argc; i++){
		if(ReadParam(argv[1])==-1){
			exit(-1);	// must include <stdlib.h>
		}
	}
	if(paramFlag==0){
		if(ReadParam("./MyEth.ini")==-1){
			exit(-1);	// must include <stdlib.h>
		}
	}
	

	printf("IP-TTL=%d\n", Param.IpTTL);
	printf("MTU=%d\n", Param.MTU);

	printf("devic=%s\n", Param.device);
	printf("+++++++++++++++++++++++++++++++++++++++\n");
	show_ifreq(Param.device);
	printf("+++++++++++++++++++++++++++++++++++++++\n");

	printf("vmac=%s\n", my_ether_ntoa_r(Param.vmac, buf1));

	return (0);
}

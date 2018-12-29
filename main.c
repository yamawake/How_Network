#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <linux/if.h>
#include "ether.h"
#include "param.h"

PARAM	Param;


int show_ifreq(char *name)
{
	int soc;
	struct ifreq	ifreq;	// must include <linux/if.h>


	if((soc=socket(AF_INET, SOCK_DGRAM, 0))==-1){
		perror("socket");
		return(-1);
	}

	strcpy(ifreq.ifr_name, name);

	if(ioctl(soc, SIOCGIFFLAGS, &ifreq)==-1){	// must include <sys/ioctl.h>
		perror("ioctl:flags");
		close(soc);	// must include <unistd.h>
		return(-1);
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

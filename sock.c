#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>	// socket()
#include <string.h>
#include <linux/if.h>
#include <sys/ioctl.h>	// ioctl()
#include <unistd.h>	// close()

int GetMacAddress(char *device, u_int8_t *hwaddr)	// must include <sys/types.h>
{
	int soc;
	struct ifreq	ifreq;	// must include <linux/if.h>
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

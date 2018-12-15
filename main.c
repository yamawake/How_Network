#include <stdio.h>
#include <sys/types.h>
#include <netinet/in.h>
#include "param.h"

PARAM	Param;


int main(int argc, char *argv[])
{
	SetDefaultParam();


	printf("IP-TTL=%d\n", Param.IpTTL);
	printf("MTU=%d\n", Param.MTU);

	printf("devic=%s\n", Param.device);
	printf("+++++++++++++++++++++++++++++++++++++++\n");
	// show_ifreq(Param.device);
	printf("+++++++++++++++++++++++++++++++++++++++\n");


	return (0);
}

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <netinet/in.h>
#include "param.h"

PARAM	Param;


int main(int argc, char *argv[])
{
	int paramFlag;
	paramFlag=0;

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
	// show_ifreq(Param.device);
	printf("+++++++++++++++++++++++++++++++++++++++\n");

	//printf("vmac=%s\n", my_ether_ntoa_r(Param.vmac, buf1));

	return (0);
}

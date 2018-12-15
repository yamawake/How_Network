#include <stdio.h>
#include <netinet/in.h>
#include "param.h"

PARAM	Param;


int main(int argc, char *argv[])
{
	printf("IP-TTL=%d\n", Param.IPTTL);
	printf("MTU=%d\n", Param.MTU);

	return (0);
}

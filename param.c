#include <stdio.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <net/ethernet.h>
#include "param.h"


extern PARAM	Param;


int SetDefaultParam()
{
	Param.MTU=DEFAULT_MTU;
	Param.IpTTL=DEFAULT_IP_TTL;

	return(0);
}

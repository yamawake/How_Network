#include <stdio.h>
#include <sys/types.h>	// u_int8_t
#include <net/ethernet.h>	// ether_header
#include "arp.h"

int ArpRecv(int soc, struct ether_header *eh, u_int8_t *data, int len)
{
	return(0);
}

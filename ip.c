#include <stdio.h>
#include <sys/types.h>
#include "ip.h"


#define IP_RECV_BUF_NO	(16)

typedef struct {
	time_t timestamp;
	int	id;
	u_int8_t data[64*1024];
	int	len;
}IP_RECV_BUF;

IP_RECV_BUF IpRecvBuf[IP_RECV_BUF_NO];

	

int IpRecvBufInit()
{
	for(int i=0; i<IP_RECV_BUF_NO; i++){
		IpRecvBuf[i].id=-1;
	}

	return(0);
}

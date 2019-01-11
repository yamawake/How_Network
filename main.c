#include <stdio.h>
#include <unistd.h>	// close()
#include <stdlib.h>	// exit()
#include <string.h>
#include <sys/types.h>
#include <sys/ioctl.h>	// ioctl()
#include <netinet/in.h>
#include <linux/if.h>
#include <arpa/inet.h>
#include <signal.h>	// signal()
#include <time.h>	// time()
#include <pthread.h>	// pthread_
#include <poll.h>	// pollfd
#include <errno.h>	// errno
#include <net/ethernet.h>
#include "ether.h"
#include "param.h"
#include "sock.h"
#include "ip.h"
#include "arp.h"


PARAM	Param;

int EndFlag=0;
int DeviceSoc;


void *MyEthThread(void *arg)
{
	struct pollfd targets[1];
	int	nready;
	u_int8_t buf[2048];
	int len;

	targets[0].fd=DeviceSoc;
	targets[0].events=POLLIN|POLLERR;

	while(EndFlag==0){
		switch((nready=poll(targets, 1, 1000))){
			case -1:
				if(errno!=EINTR){
					perror("poll");
				}
				break;
			case 0:
				break;
			default:
				if(targets[0].revents&(POLLIN|POLLERR)){
					if((len=read(DeviceSoc, buf, sizeof(buf)))<=0){
						perror("read");
						}
					else{
						EtherRecv(DeviceSoc, buf, len);
					}
					break;
				}
		}
	}

	return(NULL);
}


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


void sig_term(int sig)
{
	EndFlag=1;
}

int ending()
{
	struct ifreq	if_req;

	printf("ending\n");

	if(DeviceSoc!=-1){
		strcpy(if_req.ifr_name, Param.device);
		if(ioctl(DeviceSoc, SIOCGIFFLAGS, &if_req)<0){
			perror("ioctl");
		}

		if_req.ifr_flags=if_req.ifr_flags&~IFF_PROMISC;
		if(ioctl(DeviceSoc, SIOCSIFFLAGS, &if_req)<0){
			perror("ictl");
		}

		close(DeviceSoc);
		DeviceSoc=-1;
	}

	return(0);
}


int main(int argc, char *argv[])
{
	int paramFlag=0;
	char buf1[80];
	pthread_attr_t	attr;
	//pthread_t	thread_id;

	SetDefaultParam();

	for (int i = 1; i < argc; i++){
		if(ReadParam(argv[1])==-1){
			exit(-1);
		}
	}
	if(paramFlag==0){
		if(ReadParam("./MyEth.ini")==-1){
			exit(-1);
		}
	}
	

	printf("IP-TTL=%d\n", Param.IpTTL);
	printf("MTU=%d\n", Param.MTU);

	srandom(time(NULL));

	IpRecvBufInit();
	
	if((DeviceSoc=init_socket(Param.device))==-1){
		exit(-1);
	}

	printf("device=%s\n", Param.device);
	printf("+++++++++++++++++++++++++++++++++++++++\n");
	show_ifreq(Param.device);
	printf("+++++++++++++++++++++++++++++++++++++++\n");

	printf("vmac=%s\n", my_ether_ntoa_r(Param.vmac, buf1));
	printf("vip=%s\n", inet_ntop(AF_INET, &Param.vip, buf1, sizeof(buf1)));
	printf("vmask=%s\n", inet_ntop(AF_INET, &Param.vmask, buf1, sizeof(buf1)));
	printf("gateway=%s\n", inet_ntop(AF_INET, &Param.gateway, buf1, sizeof(buf1)));

	signal(SIGINT, sig_term);
	signal(SIGTERM, sig_term);
	signal(SIGQUIT, sig_term);
	signal(SIGPIPE, SIG_IGN);

	pthread_attr_init(&attr);
	/*
	pthread_attr_setstacksize(&attr, 102400);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	if(pthread_create(&thread_id, &attr, MyEthThread, NULL)!=0){
		printf("pthread_create:error\n");
	}
	*/


	while(EndFlag==0){
		sleep(1);
	}

	ending();


	return (0);
}

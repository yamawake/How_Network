#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <net/ethernet.h>
#include "param.h"


extern PARAM	Param;

static char *ParamFname=NULL;


int SetDefaultParam()
{
	Param.MTU=DEFAULT_MTU;
	Param.IpTTL=DEFAULT_IP_TTL;

	return(0);
}


int ReadParam(char *fname)
{
	FILE *fp;
	char buf[1024];
	char *ptr;
	char *saveptr;

	ParamFname=fname;

	if((fp=fopen(fname, "r"))==NULL){
		printf("%s cannot read\n", fname);
		return(-1);
	}

	while(1){
		fgets(buf, sizeof(buf), fp);
		if(feof(fp)){
			break;
		}

		ptr=strtok_r(buf, "=", &saveptr);

		if(ptr!=NULL){
			if(strcmp(ptr, "IP-TTL")==0){
				if((ptr=strtok_r(NULL, "\r\n", &saveptr))!=NULL){	// must include <string.h>
					Param.IpTTL=atoi(ptr);
				}
			} else if(strcmp(ptr, "MTU")==0){
				if((ptr=strtok_r(NULL,"\r\n", &saveptr))!=NULL){
					Param.MTU=atoi(ptr);	// must include <stdlib.h>
					if(Param.MTU>ETHERMTU){
						printf("ReadParam:MTU(%d) <= ETHERMTU(%d)\n", Param.MTU, ETHERMTU);
						Param.MTU=ETHERMTU;
					}
				}
			}
		}
	}

	fclose(fp);

	return(0);
}

#define	DEFAULT_MTU	(ETHERMTU)	// must inclue <net/ethernet.h>
#define	DEFAULT_IP_TTL	(64)
#define	DEFAULT_PING_SIZE	(64)


typedef struct {
	char	*device;
	u_int8_t	mymac[6];	// must include <sys/types.h>
	struct in_addr	myip;	// must include <netinet/in.h>
	struct in_addr	vmask;	// must include <netinet/in.h>
	int	IpTTL;
	int	MTU;
	struct in_addr	gateway;	// must include <netinet/in.h>
}PARAM;


int SetDefaultParam();

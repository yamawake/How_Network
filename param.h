typedef struct {
	char	*device;
	u_int8_t	mymac[6];
	struct in_addr	myip;	// must include <netinet/in.h>
	struct in_addr	vmask;	// must include <netinet/in.h>
	int	IPTTL;
	int	MTU;
	struct in_addr	gateway;	// must include <netine./in.h>
}PARAM;

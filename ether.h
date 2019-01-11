char *my_ether_ntoa_r(u_int8_t *hwaddr, char *buf);
int my_ether_aton(char *str, u_int8_t *mac);
int EtherRecv(int soc, u_int8_t *in_ptr, int in_len);

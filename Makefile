PROGRAM=MyEth
OBJS=main.o param.o ether.o sock.o ip.o
SRCS=$(OBJS:%.o=%.c)
CFLAGS=-Wall -g
LDFLAGS=-lpthread

$(PROGRAM):$(OBJS)
	$(CC) $(CFLAGS) $(ldfags) -o $(PROGRAM) $(OBJS) $(LDLIBS)

clean:
	-rm -f $(OBJS) 

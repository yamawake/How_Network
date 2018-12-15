PROGRAM=MyEth
OBJS=main.o param.o
SRCS=$(OBJS:%.o=%.c)
CFLAGS=-Wall -g
LDFLAGS=-lpthread
$(PROGRAM):$(OBJS)
	$(CC) $(CFLAGS) $(ldfags) -o $(PROGRAM) $(OBJS) $(LDLIBS)

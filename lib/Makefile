OBJS := $(patsubst %.c, %.o, $(filter %.c, $(shell ls)))


libuser.a: $(OBJS)
	ar r $@ $^

%.o: %.c
	gcc -I../include -m32 -ffreestanding -fno-pie -fno-asynchronous-unwind-tables -O2 -Wall -Wextra -Wno-unused-parameter -c -o $@ $<

clean:
	rm -f $(OBJS) libuser.a

OBJS := $(patsubst %.c, %.o, $(filter %.c, $(shell ls)))


shell: $(OBJS)
	ld -T linker.ld -o $@ $^ -L../../lib -luser 

%.o: %.c
	gcc -I../../include -m32 -ffreestanding -fno-pie -fno-asynchronous-unwind-tables -Wall -Wextra -c -o $@ $<

clean:
	rm -f $(OBJS) shell

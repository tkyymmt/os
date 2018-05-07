OBJS := $(addsuffix .o, $(filter %.s %.c, $(shell ls)))


kernel: $(OBJS)
	ld -T linker.ld -o $@ $^
	objcopy --pad-to 0x2400 -I binary $@

%.c.o: %.c
	gcc -m32 -ffreestanding -fno-pie -fno-asynchronous-unwind-tables -O2 -Wall -Wextra -c -o $@ $<

%.s.o: %.s
	as --32 -o $@ $<

clean:
	rm -f $(OBJS) kernel

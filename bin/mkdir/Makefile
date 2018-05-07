mkdir: mkdir.o
	ld -melf_i386 -Ttext 0x10000 -e main --oformat binary -o $@ $< -L../../lib -luser 

mkdir.o: mkdir.c
	gcc -I../../include -m32 -ffreestanding -fno-pie -fno-asynchronous-unwind-tables -Wall -Wextra -c $< -o $@

clean:
	rm -f mkdir.o mkdir

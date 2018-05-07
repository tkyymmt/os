ls: ls.o
	ld -melf_i386 -Ttext 0x10000 -e main --oformat binary -o $@ $< -L../../lib -luser 

ls.o: ls.c
	gcc -I../../include -m32 -ffreestanding -fno-pie -fno-asynchronous-unwind-tables -Wall -Wextra -c $< -o $@

clean:
	rm -f ls.o ls

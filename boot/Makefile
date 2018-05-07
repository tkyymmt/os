boot: boot.o
	ld -melf_i386 --oformat binary -Ttext 0x7c00 -e _start -o $@ $^
	objcopy --pad-to 0x01fe -I binary $@
	objcopy --gap-fill 0x55 --pad-to 0x01ff -I binary $@
	objcopy --gap-fill 0xaa --pad-to 0x0200 -I binary $@

boot.o: boot.s
	as --32 -o $@ $<

clean:
	rm -f boot.o boot

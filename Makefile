vfd.img: mkvfd boot/boot kernel/kernel bin
	./mkvfd $@ \
		-b boot/boot \
		-m /boot \
		-c kernel/kernel /boot \
		-m /bin \
		-c bin/shell/shell /bin \
		-c bin/mkdir/mkdir /bin \
		-c bin/rmdir/rmdir /bin \
		-c bin/mvdir/mvdir /bin \
		-c bin/ls/ls /bin

mkvfd: mkvfd.c
	gcc -o $@ $<

boot/boot:
	make -C boot
    
kernel/kernel:
	make -C kernel

.PHONY: lib
lib:
	make -C $@

.PHONY: bin
bin: lib
	make -C $@

run: vfd.img
	qemu-system-i386 -monitor stdio -m 1 -fda $<

clean:
	make $@ -C boot
	make $@ -C kernel
	make $@ -C bin
	make $@ -C lib
	rm -f vfd.img mkvfd

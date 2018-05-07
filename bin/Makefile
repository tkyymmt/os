SUBDIRS := $(shell ls -d */)


all:
	for sd in $(SUBDIRS); \
	do \
		make -C $${sd}; \
	done

clean:
	for sd in $(SUBDIRS); \
	do \
		make $@ -C $${sd}; \
	done

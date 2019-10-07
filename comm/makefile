SUBDIRS=network

all:
	@for dir in $(SUBDIRS); do \
		make all -C $$dir;    \
	done


release:
	@for dir in $(SUBDIRS); do \
		make release -C $$dir;    \
	done


clean:
	@for dir in $(SUBDIRS); do \
		make clean -C $$dir;    \
	done

install:
	

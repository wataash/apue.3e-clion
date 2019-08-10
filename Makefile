DIRS = lib libxx intro sockets advio daemons datafiles db environ \
	fileio filedir ipc1 ipc2 proc pty relation signals standards \
	stdio termios threadctl threads printer exercises

# fast
#DIRS = lib libxx advio

all: all_orig sandbox

.PHONY: all_orig
all_orig:
	for i in $(DIRS); do \
		(cd $$i && echo "making $$i" && $(MAKE) ) || exit 1; \
	done

clean:
	for i in $(DIRS); do \
		(cd $$i && echo "cleaning $$i" && $(MAKE) clean) || exit 1; \
	done
	rm -f sandbox.o sandbox

ROOT=.
EXTRALIBS = -lpthread
PLATFORM=$(shell $(ROOT)/systype.sh)
include $(ROOT)/Make.defines.$(PLATFORM)
# TODO: how to write this dependency...
# sandbox: sandbox.o all_orig
sandbox: sandbox.o

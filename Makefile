DIRS = lib libxx intro sockets advio daemons datafiles db environ \
	fileio filedir ipc1 ipc2 proc pty relation signals standards \
	stdio termios threadctl threads printer exercises

# fast
DIRS = lib libxx daemons ipc1

all:
	for i in $(DIRS); do \
		(cd $$i && echo "making $$i" && $(MAKE) ) || exit 1; \
	done

clean:
	for i in $(DIRS); do \
		(cd $$i && echo "cleaning $$i" && $(MAKE) clean) || exit 1; \
	done

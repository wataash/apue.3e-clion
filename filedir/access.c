#include "apue.h"
#include <fcntl.h>

int
main(int argc, char *argv[])
{
	if (argc != 2)
		err_quit("usage: a.out <pathname>");
	if (access(argv[1], R_OK) < 0)
		err_ret("access error for %s", argv[1]);
	else
		printf("read access OK\n");
	if (open(argv[1], O_RDONLY) < 0)
		err_ret("open error for %s", argv[1]);
	else
		printf("open for reading OK\n");

	// 0
	int rv = access("../README", F_OK);
	rv = faccessat(AT_FDCWD, "../README", F_OK, 0);
	rv = faccessat(AT_FDCWD, "../README", F_OK, AT_EACCESS|AT_SYMLINK_NOFOLLOW);
	// -1 ... because ../README is not a symbolic link?
	rv = faccessat(AT_FDCWD, "../README", F_OK, AT_SYMLINK_FOLLOW);

	exit(0);
}

#include "apue.h"
#include <errno.h>
#include <fcntl.h>

// arg: 0, 1, or 2

int
main(int argc, char *argv[])
{
	int		val;

	if (argc != 2)
		err_quit("usage: a.out <descriptor#>");

	if ((val = fcntl(atoi(argv[1]), F_GETFL, 0)) < 0)
		err_sys("fcntl error for fd %d", atoi(argv[1]));

	switch (val & O_ACCMODE) {
	case O_RDONLY:
		// macOS: 0
		printf("read only");
		{
			int n = write(atoi(argv[1]), "foo\n", 4);
			int e = errno;
			(void)EBADF;
			n = 0;
		}
		break;

	case O_WRONLY:
		// macOS: 1, 2
		printf("write only");
		{
			int n = read(atoi(argv[1]), "foo\n", 4);
			int e = errno;
			(void)EBADF;
			n = 0;
		}
		break;

	case O_RDWR:
		// linux: 0
		printf("read write");
		break;

	default:
		err_dump("unknown access mode");
	}

	if (val & O_APPEND)
		printf(", append");
	if (val & O_NONBLOCK)
		printf(", nonblocking");
	if (val & O_SYNC)
		printf(", synchronous writes");

#if !defined(_POSIX_C_SOURCE) && defined(O_FSYNC) && (O_FSYNC != O_SYNC)
	if (val & O_FSYNC)
		printf(", synchronous writes");
#endif

	putchar('\n');

	{
		val = dup2(0, 3);                        // 3
		val = fcntl(3, 0 | F_SETFD, FD_CLOEXEC); // 0
		val = dup2(0, 4);                        // 4

		// val = fcntl(0, F_GETFD, 0);
		val = fcntl(0, F_GETFD); // 0
		val = fcntl(3, F_GETFD); // 1 (FD_CLOEXEC)
		val = fcntl(4, F_GETFD); // 0 (FD_CLOEXEC cleared)

		val = fcntl(0, F_GETFL); // macos:0 linux:32770
		val = fcntl(3, F_GETFL); // macos:0 linux:32770
		val = fcntl(4, F_GETFL); // macos:0 linux:32770

		val &= ~O_NONBLOCK; // how to turn off O_NONBLOCK

		val = fcntl(0, val | F_SETFL, O_NONBLOCK); // macos:0             linux:-1
		val = fcntl(0, F_GETFL);                   // macos:4(O_NONBLOCK) linux:32770(TODO:?)
		val = fcntl(3, F_GETFL);                   // macos:4(O_NONBLOCK) linux:32770(TODO:?)
		val = fcntl(4, F_GETFL);                   // macos:4(O_NONBLOCK) linux:32770(TODO:?)
		// ref: Figure 3.9

		val = 0;
	}

	{
		// open(2):
		//   O_APPEND
		//     Before each write(2), the file offset is positioned at
		//     the end of the file, as if with lseek(2).

		int rc = system("echo foo > /tmp/foo");             // 0
		int fd = open("/tmp/foo", O_RDWR | O_APPEND, 0600); // 5
		rc = lseek(fd, 0, SEEK_CUR);                        // 0
		rc = write(fd, "bar\n", 4);  // 4 (bytes written)
		rc = lseek(fd, 0, SEEK_CUR); // 8 (4 + 4)
		close(fd);

		// /tmp/foo: foo bar\n (8 bytes)
		fd = open("/tmp/foo", O_RDWR | O_APPEND, 0600); // 5
		rc = fcntl(fd, F_GETFL); // macos:10(O_RDWR|O_APPEND) linux:33794(?)

		rc = lseek(fd, 2, SEEK_SET); // 2
		rc = write(fd, "baz\n", 4);  // 4 (bytes written)
		rc = lseek(fd, 0, SEEK_CUR); // 12 (8 + 4) (SEEK_SET ignored!)

		rc = lseek(fd, 2, SEEK_SET); // 2
		rc = write(fd, "qux\n", 4);  // 4 (bytes written)
		rc = lseek(fd, 0, SEEK_CUR); // 16 (12 + 4) (SEEK_SET ignored!)

		// /tmp/foo: foo bar baz qux\n (16 bytes)

		// macos:10(O_RDWR|O_APPEND;unchanged) linux:33794(?)
		rc = fcntl(fd, F_GETFL);

		close(fd);

		rc = 0;
	}

	exit(0);
}

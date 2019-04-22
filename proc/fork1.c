#include "apue.h"

#include <sys/param.h>
#include <sys/types.h>

#include <sys/fcntl.h>
#include <unistd.h>

int		globvar = 6;		/* external variable in initialized data */
char	buf[] = "a write to stdout\n";

int
main(void)
{
	int		var;		/* automatic variable on the stack */
	pid_t	pid;

	var = 88;
	if (write(STDOUT_FILENO, buf, sizeof(buf)-1) != sizeof(buf)-1)
		err_sys("write error");
	printf("before fork\n");	/* we don't flush stdout */

	if ((pid = fork()) < 0) {
		err_sys("fork error");
	} else if (pid == 0) {		/* child */
		globvar++;				/* modify variables */
		var++;
		exit(0);
	} else {
		sleep(2);				/* parent */
	}

	printf("pid = %ld, glob = %d, var = %d\n", (long)getpid(), globvar,
	  var);

	// p.265
	// It is important that the parent and the child share the same file
	// offset.
	{
		system("echo foo > foo");
		int fd = open("foo", O_RDONLY);    // 3
		off_t rv = lseek(fd, 0, SEEK_CUR); // 0
		printf("%05d: lseek: %jd\n", getpid(), rv);
		pid_t pid = fork();
		if (pid == 0) {
			printf("%05d: child\n", getpid());
			char buf_[100];
			int n = read(fd, buf_, 100);
			rv = lseek(fd, 0, SEEK_CUR); // 4
			printf("%05d: lseek: %jd\n", getpid(), rv);
			exit(0);
		}
		printf("%05d: parent\n", getpid());
		sleep(1);
		rv = lseek(fd, 0, SEEK_CUR); // 4; modified by child
		printf("%05d: lseek: %jd\n", getpid(), rv);
	}

	exit(0);
}

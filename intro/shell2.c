#include "apue.h"
#include <sys/wait.h>
#include <errno.h>

static void	sig_int(int);		/* our signal-catching function */

int
main(int argc, char *argv[])
{
	setbuf(stdout, NULL);

	char	buf[MAXLINE];	/* from apue.h */
	pid_t	pid;
	int		status;

	if (signal(SIGINT, sig_int) == SIG_ERR)
		err_sys("signal error");

	printf("%% ");	/* print prompt (printf requires %% to print %) */
	// macOS:
	// aaa(^C):
	//   w/ SA_RESTART: discard "aaa", fgets() again
	//   w/o          : returns NULL with EINTR
	// set/unset breakpoint: returns NULL with EINTR
	while (fgets(buf, MAXLINE, stdin) != NULL) {
		if (buf[strlen(buf) - 1] == '\n')
			buf[strlen(buf) - 1] = 0; /* replace newline with null */

		if ((pid = fork()) < 0) {
			err_sys("fork error");
		} else if (pid == 0) {		/* child */
			execlp(buf, buf, (char *)0);
			err_ret("couldn't execute: %s", buf);
			exit(127);
		}

		/* parent */
		if ((pid = waitpid(pid, &status, 0)) < 0)
			err_sys("waitpid error");
		printf("%% ");
	}
	int e = errno;
	const char* s = strerror(errno);

	// macOS:
	// 4 EINTR
	// ./intro/shell2: Interrupted system call
	printf("errno: %d\n", e);
	perror(argv[0]);
	exit(0);
}

void
sig_int(int signo)
{
	printf("interrupt\n%% ");
}

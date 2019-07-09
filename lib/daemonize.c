#include "apue.h"
#include <errno.h>
#include <syslog.h>
#include <fcntl.h>
#include <sys/resource.h>

// try in: ipc1/pipe1.c

void
daemonize(const char *cmd)
{
	int i, fd0, fd1, fd2;
	pid_t pid;
	struct rlimit rl;
	struct sigaction sa;

	/*
	 * Clear file creation mask.
	 */
	umask(0);

	/*
	 * Get maximum number of file descriptors.
	 */
	if (getrlimit(RLIMIT_NOFILE, &rl) < 0)
		err_quit("%s: can't get file limit", cmd);

	// (gdb) set follow-fork-mode child
	// (gdb) set detach-on-fork off
	/*
	 * Become a session leader to lose controlling TTY.
	 */
	printf("%05d: %s\n", getpid(), __func__);
	// only `set follow-fork-mode child` -> die by SIGHUP
	// (because CLion closes it?)
	// (gdb) info inferiors
	//   Num  Description       Executable
	//   1    process 17532     /home/wsh/src/apue.3e-clion/ipc1/pipe1
	// * 2    process 17554     /home/wsh/src/apue.3e-clion/ipc1/pipe1
	// (gdb) detach infe 1
	// -> clion finishes even if debugging child
	if ((pid = fork()) < 0)
		err_quit("%s: can't fork", cmd);
	else if (pid != 0) /* parent */
	{
		printf("%05d: %s parent: exit\n", getpid(), __func__);
		exit(0);
	}
	printf("%05d: %s child\n", getpid(), __func__);
	int rv = getsid(getpid());
	rv = \
	setsid();
	int e = errno;
	rv = getsid(getpid());

	/*
	 * Ensure future opens won't allocate controlling TTYs.
	 */
	sa.sa_handler = SIG_IGN;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	if (sigaction(SIGHUP, &sa, NULL) < 0)
		err_quit("%s: can't ignore SIGHUP", cmd);
	printf("%05d: %s child: fork\n", getpid(), __func__);
	if ((pid = fork()) < 0)
		err_quit("%s: can't fork", cmd);
	else if (pid != 0) /* parent */
	{
		printf("%05d: %s child parent: exit\n", getpid(), __func__);
		exit(0);
	}

	printf("%05d: %s child child\n", getpid(), __func__);

	/*
	 * Change the current working directory to the root so
	 * we won't prevent file systems from being unmounted.
	 */
	if (chdir("/") < 0)
		err_quit("%s: can't change directory to /", cmd);

	/*
	 * Close all open file descriptors.
	 */
	if (rl.rlim_max == RLIM_INFINITY)
		rl.rlim_max = 1024;
	for (i = 0; i < rl.rlim_max; i++)
		close(i);

	/*
	 * Attach file descriptors 0, 1, and 2 to /dev/null.
	 */
	fd0 = open("/dev/null", O_RDWR);
	fd1 = dup(0);
	fd2 = dup(0);

	/*
	 * Initialize the log file.
	 */
	openlog(cmd, LOG_CONS, LOG_DAEMON);
	if (fd0 != 0 || fd1 != 1 || fd2 != 2) {
		syslog(LOG_ERR, "unexpected file descriptors %d %d %d",
		  fd0, fd1, fd2);
		exit(1);
	}
}

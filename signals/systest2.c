#include "apue.h"
#include <errno.h>
#include <wait.h>

int system_(const char *cmdstring);

static void
sig_int(int signo)
{
	printf("caught SIGINT\n");
}

static void
sig_chld(int signo)
{
	printf("caught SIGCHLD\n");
}

int
main(void)
{
	int rc;
	errno = 0;
	rc = system("no_such_command"); // 32512; sh: 1: no_such_command: not found
	err_ret("system");              // Success
	errno = 0;
	rc = system_("no_such_command"); // 32512; sh: 1: no_such_command: not found
	err_ret("system_");              // Success
	pr_exit(rc);                     // normal termination, exit status = 127

	if (signal(SIGINT, sig_int) == SIG_ERR)
		err_sys("signal(SIGINT) error");
	if (signal(SIGCHLD, sig_chld) == SIG_ERR)
		err_sys("signal(SIGCHLD) error");
	if (system_("/bin/ed") < 0)
		err_sys("system() error");
	exit(0);
}

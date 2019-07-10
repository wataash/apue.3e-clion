// try in sandbox.c; comment out already_running()

#include "apue.h"
#include <syslog.h>
#include <errno.h>

extern int lockfile(int);
extern int already_running(void);

void
reread(void)
{
	/* ... */
}

// in clion: can't catch SIGTERM?
void
sigterm(int signo)
{
	pr_mask("sigterm"); // mask: HUP TERM
	syslog(LOG_INFO, "got SIGTERM; exiting");
	exit(0);
}

void
sighup(int signo)
{
	pr_mask("sighup"); // mask: HUP TERM
	syslog(LOG_INFO, "Re-reading configuration file");
	reread();
}

int
main(int argc, char *argv[])
{
	char				*cmd;
	struct sigaction	sa;

	if ((cmd = strrchr(argv[0], '/')) == NULL)
		cmd = argv[0];
	else
		cmd++;

	/*
	 * Become a daemon.
	 */
	// daemonize(cmd);

	/*
	 * Make sure only one copy of the daemon is running.
	 */
	// if (already_running()) {
	// 	syslog(LOG_ERR, "daemon already running");
	// 	exit(1);
	// }

	/*
	 * Handle signals of interest.
	 */
	sa.sa_handler = sigterm;
	sigemptyset(&sa.sa_mask);
	sigaddset(&sa.sa_mask, SIGHUP);
	sa.sa_flags = 0;
	if (sigaction(SIGTERM, &sa, NULL) < 0) {
		syslog(LOG_ERR, "can't catch SIGTERM: %s", strerror(errno));
		exit(1);
	}
	sa.sa_handler = sighup;
	sigemptyset(&sa.sa_mask);
	sigaddset(&sa.sa_mask, SIGTERM);
	sa.sa_flags = 0;
	if (sigaction(SIGHUP, &sa, NULL) < 0) {
		syslog(LOG_ERR, "can't catch SIGHUP: %s", strerror(errno));
		exit(1);
	}

	for (;;) {
		int n = read(0, NULL, 1);
		int e = errno; // EINTR or EFAULT
		e = 0;
		(void)EINTR;
		(void)EFAULT;
	}

	/*
	 * Proceed with the rest of the daemon.
	 */
	/* ... */
	exit(0);
}

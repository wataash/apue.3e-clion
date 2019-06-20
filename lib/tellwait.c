#include "apue.h"

static volatile sig_atomic_t sigflag; /* set nonzero by sig handler */
static sigset_t newmask, oldmask, zeromask;
static sigset_t tmpmask;

// doesn't care SIGUSR[12] from other processes; it causes deadlock

static void
sig_usr(int signo)	/* one signal handler for SIGUSR1 and SIGUSR2 */
{
	// only handled signal (SIGUSR[12])
	pr_mask("sig_usr (may race...)     ");
	sigflag = 1;
}

void
TELL_WAIT(void)
{
	pr_mask("TELL_WAIT");

	if (signal(SIGUSR1, sig_usr) == SIG_ERR)
		err_sys("signal(SIGUSR1) error");
	if (signal(SIGUSR2, sig_usr) == SIG_ERR)
		err_sys("signal(SIGUSR2) error");

	// deadlock if SIGUSR[12] by terminal here

	sigemptyset(&zeromask);
	sigemptyset(&newmask);
	sigaddset(&newmask, SIGUSR1);
	sigaddset(&newmask, SIGUSR2);

	// SA_REATART test
	// TODO: move to proper place
	if (0) {
		signal_(SIGINT, sig_usr);
		char buf[128];
		ssize_t n = read(0, buf, 128); // SA_REATART: ^C not return
		perror("read"); // no SA_RESTART: EINTR
		exit(1);
	}

	/* Block SIGUSR1 and SIGUSR2, and save current signal mask */
	if (sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0)
		err_sys("SIG_BLOCK error");

	pr_mask("TELL_WAIT return");
}

void
TELL_PARENT(pid_t pid)
{
	pr_mask("\x1b[33m TELL_PARENT              \x1b[0m");
	kill(pid, SIGUSR2);		/* tell parent we're done */
	pr_mask("\x1b[33m TELL_PARENT return       \x1b[0m");
}

void
WAIT_PARENT(void)
{
	pr_mask("\x1b[33m WAIT_PARENT              \x1b[0m");

	while (sigflag == 0)
		sigsuspend(&zeromask);	/* and wait for parent */
	sigflag = 0;

	pr_mask("\x1b[33m WAIT_PARENT sigsuspended \x1b[0m");

	/* Reset signal mask to original value */
	if (sigprocmask(SIG_SETMASK, &oldmask, &tmpmask) < 0)
		err_sys("SIG_SETMASK error");

	pr_mask("\x1b[33m WAIT_PARENT return       \x1b[0m");
}

void
TELL_CHILD(pid_t pid)
{
	pr_mask("\x1b[32m TELL_CHILD               \x1b[0m");
	kill(pid, SIGUSR1);			/* tell child we're done */
	pr_mask("\x1b[32m TELL_CHILD  return       \x1b[0m");
}

void
WAIT_CHILD(void)
{
	pr_mask("\x1b[32m WAIT_CHILD               \x1b[0m");

	while (sigflag == 0)
		sigsuspend(&zeromask);	/* and wait for child */
	sigflag = 0;

	pr_mask("\x1b[32m WAIT_CHILD  sigsuspended \x1b[0m");

	// ?
	/* Reset signal mask to original value */
	if (sigprocmask(SIG_SETMASK, &oldmask, &tmpmask) < 0)
		err_sys("SIG_SETMASK error");

	pr_mask("\x1b[32m WAIT_CHILD  return       \x1b[0m");
}

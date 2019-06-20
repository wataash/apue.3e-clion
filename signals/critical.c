#include "apue.h"

static void	sig_quit(int);

int
main(void)
{
	sigset_t	newmask, oldmask, pendmask;

	if (signal(SIGQUIT, sig_quit) == SIG_ERR)
		err_sys("can't catch SIGQUIT");

	/*
	 * Block SIGQUIT and save current signal mask.
	 */
	sigemptyset(&newmask);
	sigaddset(&newmask, SIGQUIT);
	if (sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0)
		err_sys("SIG_BLOCK error");

	// first-sleep  unblock            second-sleep
	// ^\           sig_quit()         ^\ -> coredump
	//                                 ^\ -> sig_quit()  ^\ -> no coredump

	// sleep(5);	/* SIGQUIT here will remain pending */
	for (size_t i = 0; i < 5; i++) {
		pr_mask("sleep 1");
		sleep(1);
	}
	pr_mask("sleep 1 done");

	if (sigpending(&pendmask) < 0)
		err_sys("sigpending error");
	if (sigismember(&pendmask, SIGQUIT))
		printf("\nSIGQUIT pending\n");

	pr_mask("sigispending() done");

	/*
	 * Restore signal mask which unblocks SIGQUIT.
	 */
	if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0)
		err_sys("SIG_SETMASK error");
	printf("SIGQUIT unblocked\n");

	// sleep(5);	/* SIGQUIT here will terminate with core file */
	for (size_t i = 0; i < 5; i++) {
		pr_mask("sleep 2");
		sleep(1);
	}
	pr_mask("sleep 2 done");

	exit(0);
}

static void
sig_quit(int signo)
{
	printf("caught SIGQUIT\n");
	if (signal(SIGQUIT, SIG_DFL) == SIG_ERR)
		err_sys("can't reset SIGQUIT");
}

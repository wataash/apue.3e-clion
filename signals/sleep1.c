#include	"apue.h"
#include	<signal.h>
#include	<stdio.h>
#include	<unistd.h>

static void
sig_alrm(int signo)
{
	/* nothing to do, just return to wake up the pause */
	printf("sig_alrm\n");
}

unsigned int
sleep1(unsigned int seconds)
{
	if (signal_(SIGALRM, sig_alrm) == SIG_ERR)
		return(seconds);
	int rv = alarm(seconds);		/* start the timer */
	rv = alarm(seconds);
	rv = alarm(seconds);
	rv = alarm(seconds);
	rv = alarm(seconds);

	printf("sleep1: pause()\n");
	// deadlock if already alarmed
	// returns when sig_int() finished
	// not returns if SIGINT while sig_int()
	int err = pause();			/* next caught signal wakes us up */
	printf("sleep1: pause() returned %d\n", err);
	perror("sleep1: errno");

	unsigned int rc = alarm(0);
	// rc = alarm(0); // always 0
	printf("sleep1: return %u\n", rc);
	return(rc);	/* turn off timer, return unslept time */
}

#include	"apue.h"
#include	<errno.h>
#include	<setjmp.h>
#include	<signal.h>
#include	<stdio.h>
#include	<unistd.h>

static jmp_buf	env_alrm;

static void
sig_alrm(int signo)
{
	longjmp(env_alrm, 1);
}

unsigned int
sleep2(unsigned int seconds)
{
	if (signal_(SIGALRM, sig_alrm) == SIG_ERR)
		return(seconds);

	printf("sleep2: setjmp()\n");
	if (setjmp(env_alrm) == 0) {
		alarm(seconds);		/* start the timer */
		// breaks if alarm fired
		printf("sleep2: going into pause()\n");
		// returns on sig_int() finished
		int err = pause();			/* next caught signal wakes us up */
		printf("sleep2: pause() returned %d, errno %d\n", err, errno);
	}

	return(alarm(0));		/* turn off timer, return unslept time */
}

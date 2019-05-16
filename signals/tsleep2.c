#include "apue.h"
#include <setjmp.h>
#include <stdint.h>

unsigned int	sleep1(unsigned int);
unsigned int	sleep2(unsigned int);
static void		sig_int(int);

int
main(void)
{
	{

		jmp_buf env;
		int i = 1;
		int j = 2;
		int rv = setjmp(env); // 0, 9
		i = 3;
		j = 4;
		int k = 5;
		if (rv == 0)
			longjmp(env, 9);
	}

	unsigned int	unslept;

	printf("PID: %jd\n", (intmax_t)getpid());

	if (signal(SIGINT, sig_int) == SIG_ERR)
		err_sys("signal(SIGINT) error");
	printf("going into sleep[12]\n");
	// unslept = sleep1(5);
	unslept = sleep2(5);
	printf("sleep[12] returned: %u\n", unslept);
	exit(0);
}

static void
sig_int(int signo)
{
	int				i, j;
	volatile int	k;

	/*
	 * Tune these loops to run for more than 5 seconds
	 * on whatever system this test program is run.
	 */
	printf("\nsig_int starting\n");
	for (i = 0; i < 300000; i++)
		for (j = 0; j < 4000; j++)
			k += i * j;
	printf("sig_int finished\n");
}

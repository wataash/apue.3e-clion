#include "apue.h"
#include <errno.h>

void
pr_mask(const char *str)
{
	sigset_t	sigset;
	int			errno_save;

	static const struct {
		unsigned int sig;
		const char *name;
	} pairs_sig_name[] = {
		{.sig = SIGHUP, .name = "SIGHUP"},
		{.sig = SIGINT, .name = "SIGINT"},
		{.sig = SIGQUIT, .name = "SIGQUIT"},
		{.sig = SIGILL, .name = "SIGILL"},
		{.sig = SIGTRAP, .name = "SIGTRAP"},
		{.sig = SIGABRT, .name = "SIGABRT"},
		{.sig = SIGFPE, .name = "SIGFPE"},
		{.sig = SIGKILL, .name = "SIGKILL"},
		{.sig = SIGBUS, .name = "SIGBUS"},
		{.sig = SIGSEGV, .name = "SIGSEGV"},
		{.sig = SIGSYS, .name = "SIGSYS"},
		{.sig = SIGPIPE, .name = "SIGPIPE"},
		{.sig = SIGALRM, .name = "SIGALRM"},
		{.sig = SIGTERM, .name = "SIGTERM"},
		{.sig = SIGURG, .name = "SIGURG"},
		{.sig = SIGSTOP, .name = "SIGSTOP"},
		{.sig = SIGTSTP, .name = "SIGTSTP"},
		{.sig = SIGCONT, .name = "SIGCONT"},
		{.sig = SIGCHLD, .name = "SIGCHLD"},
		{.sig = SIGTTIN, .name = "SIGTTIN"},
		{.sig = SIGTTOU, .name = "SIGTTOU"},
		{.sig = SIGPOLL, .name = "SIGPOLL"},
		{.sig = SIGXCPU, .name = "SIGXCPU"},
		{.sig = SIGXFSZ, .name = "SIGXFSZ"},
		{.sig = SIGVTALRM, .name = "SIGVTALRM"},
		{.sig = SIGPROF, .name = "SIGPROF"},
		{.sig = SIGWINCH, .name = "SIGWINCH"},
		{.sig = SIGUSR1, .name = "SIGUSR1"},
		{.sig = SIGUSR2, .name = "SIGUSR2"},
	};

	(void)sigemptyset(&sigset);
	errno_save = errno;		/* we can be called by signal handlers */
	if (sigprocmask(0, NULL, &sigset) < 0) {
		err_ret("sigprocmask error");
	} else {
		printf("%s", str);
		printf("\n");
		printf("  sigprocmask:");
		for (size_t i = 0; i < _SIGSET_NWORDS; i++) {
			if (sigset.__val[i] != 0)
				// printf(" [%zu]0x%0" sizeof(sigset.__val[0]) "lx(%lu)", i, sigset.__val[i], sigset.__val[i]);
				printf("  [%zu]0x%08lx(%4lu)", i, sigset.__val[i], sigset.__val[i]);
		}
		for (size_t i = 0; i < (sizeof(pairs_sig_name)/sizeof(pairs_sig_name[0])); i++) {
			unsigned int sig = pairs_sig_name[i].sig;
			const char *name = pairs_sig_name[i].name;
			if (sigismember(&sigset, pairs_sig_name[i].sig))
				printf("  %d(<<:0x%08x)%s", sig, 1u<<(sig-1), name);
		}

		// if (sigismember(&sigset, SIGINT))
		// 	printf(" SIGINT");
		// if (sigismember(&sigset, SIGQUIT))
		// 	printf(" SIGQUIT");
		// if (sigismember(&sigset, SIGUSR1))
		// 	printf(" SIGUSR1");
		// if (sigismember(&sigset, SIGALRM))
		// 	printf(" SIGALRM");

		/* remaining signals can go here  */

		printf("\n");

		(void)sigpending(&sigset);
		printf("\x1b[37m  sigispending:");
		for (size_t i = 0; i < _SIGSET_NWORDS; i++) {
			if (sigset.__val[i] != 0)
				printf(" [%zu]0x%08lx(%4lu)", i, sigset.__val[i], sigset.__val[i]);
		}
		for (size_t i = 0; i < (sizeof(pairs_sig_name)/sizeof(pairs_sig_name[0])); i++) {
			unsigned int sig = pairs_sig_name[i].sig;
			const char *name = pairs_sig_name[i].name;
			if (sigismember(&sigset, pairs_sig_name[i].sig))
				printf("  %d(<<:0x%08x)%s", sig, 1u<<(sig-1), name);
		}
		printf("\x1b[0m\n");
	}

	errno = errno_save;		/* restore errno */
}

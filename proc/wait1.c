#include "apue.h"
#include <bits/types/siginfo_t.h>
#include <errno.h>
#include <sys/resource.h>
#include <sys/wait.h>

int
main(void)
{
	pid_t	pid;
	int		status;

	if ((pid = fork()) < 0)
		err_sys("fork error");
	else if (pid == 0)				/* child */
		exit(7);

	if (wait(&status) != pid)		/* wait for child */
		err_sys("wait error");
	// normal termination, exit status = 7
	pr_exit(status);				/* and print its status */

	if ((pid = fork()) < 0)
		err_sys("fork error");
	else if (pid == 0)				/* child */
		abort();					/* generates SIGABRT */

	if (wait(&status) != pid)		/* wait for child */
		err_sys("wait error");
	pr_exit(status);				/* and print its status */

	if ((pid = fork()) < 0)
		err_sys("fork error");
	else if (pid == 0)				/* child */
		status /= 0;				/* divide by 0 generates SIGFPE */

	if (wait(&status) != pid)		/* wait for child */
		err_sys("wait error");
	pr_exit(status);				/* and print its status */

	// p.277 8.7. waitid Function
	{
		if (fork() == 0) {
			exit(9);
		}
		siginfo_t info;
		(void)P_ALL;
		(void)P_PID;
		(void)P_PGID;
		int rv = waitid(P_ALL, 0, &info,
			   WNOHANG | WNOWAIT | WSTOPPED | WEXITED | WCONTINUED);
		int e = errno;
		pid_t pid_ = info.si_pid;    // info._sifields._kill.si_pid
		uid_t uid = info.si_uid;     // info._sifields._kill.si_uid
		int signo = info.si_signo;   // SIGCHLD
		int status = info.si_status; // 9; info._sifields._sigchld.si_status
		int code = info.si_code;     // CLD_EXITED
		(void)(CLD_EXITED | CLD_KILLED | CLD_DUMPED | CLD_STOPPED | CLD_TRAPPED | CLD_CONTINUED);
		info = (siginfo_t){};
	}

	// p.278 8.8. wait3 and wait4 Functions
	{
		if (fork() == 0) {
			system("timeout 0.1 yes >/dev/null");
			exit(9);
		}

		int status;
		struct rusage rusage;
		wait3(&status, WNOHANG | WUNTRACED | WCONTINUED, &rusage);
		wait4(-1, &status, WNOHANG | WUNTRACED | WCONTINUED, &rusage);
		status = 0;

		// TODO: getrusage(2)
	}

	exit(0);
}

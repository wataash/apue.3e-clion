#include "apue.h"
#include <errno.h>
#include <sys/wait.h>

int
main(void)
{
	pid_t	pid;

	printf("0 %05d\n", getpid());

	if ((pid = fork()) < 0) {
		err_sys("fork error");
	} else if (pid == 0) {		/* first child */
		printf("1 %05d\n", getpid());
		if ((pid = fork()) < 0)
			err_sys("fork error");
		else if (pid > 0) {
			printf("%05d: sleep\n", getpid());
			sleep(4);
			printf("%05d: exit(0)\n", getpid());
			exit(0);	/* parent from second fork == first child */
		}

		printf("2 %05d\n", getpid());
		printf("%05d: sleep\n", getpid());
		/*
		 * We're the second child; our parent becomes init as soon
		 * as our real parent calls exit() in the statement above.
		 * Here's where we'd continue executing, knowing that when
		 * we're done, init will reap our status.
		 */
		sleep(2);
		printf("second child, parent pid = %ld\n", (long)getppid());
		printf("%05d: exit(0)\n", getpid());
		exit(0);
	}

	if (1) {
		int rv = waitpid(1, NULL, 0); // -1
		int e = errno;                // linux: 10
		perror("waitpid");            // waitpid: No child processes

		// rv = waitpid(0, NULL, 0); // block; same as wait()
		// e = errno;
		// perror("waitpid");

		// we can't wait second child
		int j = -1;               // change me with gdb
		rv = waitpid(j, NULL, 0); // -1
		e = errno;                // linux: 10
		perror("waitpid");        // waitpid: No child processes

		printf("%05d: wait()\n", getpid());
		pid_t pid_wait = wait(NULL);
		printf("%05d: %d\n", getpid(), pid_wait); // first child
		pid_wait = wait(NULL);
		printf("%05d: %d\n", getpid(), pid_wait); // -1
	}

	if (waitpid(pid, NULL, 0) != pid)	/* wait for first child */
		err_sys("waitpid error");

	/*
	 * We're the parent (the original process); we continue executing,
	 * knowing that we're not the parent of the second child.
	 */
	exit(0);
}

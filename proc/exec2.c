#include "apue.h"
#include <errno.h>
#include <pwd.h>
#include <sys/wait.h>

void
prid(void)
{
	uid_t ruid, euid, suid;
	gid_t rgid, egid, sgid;

	int err = getresuid(&ruid, &euid, &suid);
	if (err != 0)
		err_sys("getresuid returned %d", -1);
	if (ruid != getuid())
		err_quit("ruid:%d != getuid():%d", ruid, getuid());

	err = getresgid(&rgid, &egid, &sgid); // 0
	if (err != 0)
		err_sys("getresuid returned %d", -1);
	if (rgid != getgid())
		err_quit("rgid:%d != getgid():%d", rgid, getgid());

	puts("------------------------------");
	printf("ruid:%d euid:%d suid:%d\n", ruid, euid, suid);
	printf("rgid:%d egid:%d sgid:%d\n", rgid, egid, sgid);
	puts("------------------------------");
}

int
main(void)
{
	pid_t	pid;

	if ((pid = fork()) < 0) {
		err_sys("fork error");
	} else if (pid == 0) {			/* child */
		if (execl("/home/sar/bin/testinterp",
				  "testinterp", "myarg1", "MY ARG2", (char *)0) < 0)
			err_sys("execl error");
	}
	if (waitpid(pid, NULL, 0) < 0)	/* parent */
		err_sys("waitpid error");


	// p.289 8.11. Changing User IDs and Group IDs
	{
		int err;
		struct passwd *pwd;

		(void)_POSIX_SAVED_IDS;
		long sc_saved_ids = sysconf(_SC_SAVED_IDS);

		pwd = getpwnam("foo"); // NULL
		int e = errno;         // 0

		pwd = getpwnam("wsh");
		(void)pwd->pw_name;   // wsh
		(void)pwd->pw_passwd; // x
		(void)pwd->pw_uid;    // 1000
		(void)pwd->pw_gid;    // 1000
		(void)pwd->pw_gecos;  // wsh,,,
		(void)pwd->pw_dir;    // /home/wsh
		(void)pwd->pw_shell;  // /bin/bash

		pwd = getpwnam("sshd");
		(void)pwd->pw_name;   // sshd
		(void)pwd->pw_passwd; // x
		(void)pwd->pw_uid;    // 122
		(void)pwd->pw_gid;    // 65534
		(void)pwd->pw_gecos;  //
		(void)pwd->pw_dir;    // /run/sshd
		(void)pwd->pw_shell;  // /usr/sbin/nologin

		// aborted; do with python

		// all 1000
		prid();

		// sudo chown sshd:nogroup exec2; sudo chmod +s exec2
		// sudo chown root:root exec2; sudo chmod +s exec2
		// ./exec2
		//
		// -rwsr-sr-x 1 sshd nogroup 102368  5月  7 18:27 exec2*
		//
		// ruid:1000 euid:122 suid:122
		// rgid:1000 egid:65534 sgid:65534
		//
		// sudo ./exec2
		//
		// ruid:0 euid:122 suid:122
		// rgid:0 egid:65534 sgid:65534

		err = setgid(0);
		if (err != 0)
			err_ret("setgid(0)");
		prid();

		err = setgid(65534);
		if (err != 0)
			err_ret("setgid(65534)");
		prid();

		err = setgid(0);
		if (err != 0)
			err_ret("setgid(0)");
		prid();

		err = setuid(0);
		if (err != 0)
			err_ret("setuid(0)");
		prid();

		err = setuid(122);
		if (err != 0)
			err_ret("setuid(122)");
		prid();

		err = setuid(0);
		if (err != 0)
			err_ret("setuid(0)");
		prid();

		err = setgid(65534);
		if (err != 0)
			err_ret("setgid(65534)");
		prid();

		err = 0;
	}

	exit(0);
}

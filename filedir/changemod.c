#include "apue.h"

int
main(void)
{
	struct stat		statbuf;

	/* turn on set-group-ID and turn off group-execute */

	system("touch foo");
	system("stat foo"); // 0644

	if (stat("foo", &statbuf) < 0)
		err_sys("stat error for foo");
	// remove S_IXGRP, append S_ISGID
	if (chmod("foo", (statbuf.st_mode & ~S_IXGRP) | S_ISGID) < 0)
		err_sys("chmod error for foo");

	system("stat foo"); // 2644

	/* set absolute mode to "rw-r--r--" */

	umask(0777);
	system("touch bar");
	system("stat bar"); // 0000

	// ok even in umask(0777)
	if (chmod("bar", S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH) < 0)
		err_sys("chmod error for bar");

	system("stat bar"); // 0644

	system("rm foo bar");

	exit(0);
}

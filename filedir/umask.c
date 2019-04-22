#include "apue.h"
#include <fcntl.h>

#define RWRWRW (S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH)

int
main(void)
{
	mode_t mode = umask(0); // 18 (old value) (gdb p/o => 0022)
	mode = umask(0);        // 0
	mode = umask(0777);     // 0
	mode = umask(0);        // 511 (0777)
	mode = umask(0);        // 0
	if (creat("foo", RWRWRW) < 0)
		err_sys("creat error for foo");
	umask(S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
	if (creat("bar", RWRWRW) < 0)
		err_sys("creat error for bar");

	// -rw------- 1 wsh wsh 0  4月 28 15:46 bar
	// -rw-rw-rw- 1 wsh wsh 0  4月 28 15:46 foo
	system("ls -Fl foo bar");
	system("rm foo bar");

	exit(0);
}

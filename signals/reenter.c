#include "apue.h"
#include <pwd.h>

// in getpwnam(): libc/nss/getXXbyYY.c
// (gdb) p &resbuf
// $4 = (struct passwd *) 0x7ffff7bcaee0 <resbuf>
//
// note that getpwnam() also defined in datafiles/getpwnam.c

static void
my_alarm(int signo)
{
	struct passwd	*rootptr;

	printf("in signal handler\n");
	if ((rootptr = getpwnam("root")) == NULL)
			err_sys("getpwnam(root) error");
	// (gdb) p rootptr
	// $2 = (struct passwd *) 0x7ffff7bcaee0 <resbuf>
	//                        ^^^^^^^^^^^^^^
	alarm(1);
}

int
main(void)
{
	struct passwd	*ptr;

	signal_(SIGALRM, my_alarm);
	alarm(1);
	for ( ; ; ) {
		if ((ptr = getpwnam("wsh")) == NULL)
			err_sys("getpwnam error");
		// (gdb) p ptr
		// $1 = (struct passwd *) 0x7ffff7bcaee0 <resbuf>
		//                        ^^^^^^^^^^^^^^
		if (strcmp(ptr->pw_name, "wsh") != 0)
			printf("return value corrupted!, pw_name = %s\n",
					ptr->pw_name);
		system("sleep 0.3");
	}
}

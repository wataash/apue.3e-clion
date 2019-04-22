#include "apue.h"
#include <fcntl.h>

char	buf1[] = "abcdefghij";
char	buf2[] = "ABCDEFGHIJ";

int
main(void)
{
	int		fd;

	if ((fd = creat("file.hole", FILE_MODE)) < 0)
		err_sys("creat error");

	if (write(fd, buf1, 10) != 10)
		err_sys("buf1 write error");
	/* offset now = 10 */


	system("xxd file.hole");
	// macOS:
	// 00000000: 6162 6364 6566 6768 696a                 abcdefghij
	system("ls -ls file.hole");
	// linux:
	// 4 -rw-r--r-- 1 wsh wsh 10  4月 28 15:54 file.hole
	// ^ 4 blocks             ^^ 10B
	// macOS:
	// 8 -rw-r--r--  1 wsh  staff  10 Apr 24 13:59 file.hole
	// ^ 8 blocks (*512B = 4KiB)   ^^ 10B

	if (lseek(fd, 16384, SEEK_SET) == -1)
		err_sys("lseek error");
	/* offset now = 16384 */

	if (write(fd, buf2, 10) != 10)
		err_sys("buf2 write error");
	/* offset now = 16394 */

	system("xxd file.hole | head");
	system("xxd file.hole | tail");
	// 00000000: 6162 6364 6566 6768 696a 0000 0000 0000  abcdefghij......
	// ...
	// 00003ff0: 0000 0000 0000 0000 0000 0000 0000 0000  ................
	// 00004000: 4142 4344 4546 4748 494a                 ABCDEFGHIJ
	system("ls -ls file.hole");
	// linux:
	// 8 -rw-r--r-- 1 wsh wsh 16394  4月 28 15:56 file.hole
	// ^ 8 blocks             ^^^^^ 16.01 KiB
	// macOS:
	// 40 -rw-r--r--  1 wsh  staff  16394 Apr 24 13:57 file.hole
	// ^^ 40 blocks (*512B = 20KiB) ^^^^^ 16.01 KiB

	system("rm file.hole");

	exit(0);
}

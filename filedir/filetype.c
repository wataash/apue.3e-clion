#include "apue.h"
#include <fcntl.h>

/*
https://stackoverflow.com/q/38802364/4085441
S_I : Stat {Integer, Identifier, Inode}?

.                  FMT UGV  mode                   .
.                  /^^\^^^/^^^^^^^\                .
.  10987654321098765432109876543210                .
.                               ^^^ S_IRWXO        .
.                               ^   S_IROTH        .
.                                ^  S_IWOTH        .
.                                 ^ S_IXOTH        .
.                            ^^^    S_IRWXG        .
.                            ^      S_IRGRP        .
.                             ^     S_IWGRP        .
.                              ^    S_IXGRP        .
.                         ^^^       S_IRWXU        .
.                         ^         S_IRUSR        .
.                          ^        S_IWUSR        .
.                           ^       S_IXUSR        .
.                        ^ S_ISVTX saved text      .
.                       ^  S_ISGID                 .
.                      ^   S_ISUID                 .
.                  ^^^^ S_IFMT    format?          .
.                     ^ S_IFIFO   fifo             .
.                    ^  S_IFCHR   file character   .
.                   ^   S_IFDIR   file directory   .
.                   ^^  S_IFBLK   file block       .
.                  ^    S_IFREG   file regular     .
.                  ^ ^  S_IFLINK  file link        .
.                  ^^   S_IFSOCK  file socket      .
.                                                  .

(gdb )p/t 0170000 => 0b00000000000000001111000000000000

S_ISREG Stat Is Regular ?
 */

int
main(int argc, char *argv[])
{
	int			i;
	struct stat	buf;
	char		*ptr;

	for (i = 1; i < argc; i++) {
		printf("%s: ", argv[i]);
		if (lstat(argv[i], &buf) < 0) {
			err_ret("lstat error");
			continue;
		}
		if (S_ISREG(buf.st_mode))
			ptr = "regular";
		else if (S_ISDIR(buf.st_mode))
			ptr = "directory";
		else if (S_ISCHR(buf.st_mode))
			ptr = "character special";
		else if (S_ISBLK(buf.st_mode))
			ptr = "block special";
		else if (S_ISFIFO(buf.st_mode))
			ptr = "fifo";
		else if (S_ISLNK(buf.st_mode))
			ptr = "symbolic link";
		else if (S_ISSOCK(buf.st_mode))
			ptr = "socket";
		else
			ptr = "** unknown mode **";
		printf("%s\n", ptr);

		int j = major(buf.st_dev);
		j = minor(buf.st_dev);

		// macOS, linux: 0; not implemented
		j = S_TYPEISMQ(&buf);  // is message queue?
		j = S_TYPEISSEM(&buf); // is semaphore
		j = S_TYPEISSHM(&buf); // is shared memory
#ifdef __APPLE__
		// [TYM] typed memory object
		j = S_TYPEISTMO(&buf); // is typed memory object
#endif

		S_BLKSIZE;

		j = 0;
	}

	{
		int err = system("touch foo");  // 0
		err = stat("foo", &buf);        // 0
		int fd = open("foo", O_RDONLY); // 3
		if (fd == -1)
			err_sys("open foo");

		err = fstat(fd, &buf);                  // 0
		err = system("ls -lF /proc/self/fd/3"); // 0
		// lr-x------ 1 wsh wsh 64  5月  9 19:13 /proc/self/fd/3 -> /home/wsh/src/apue.3e-clion/filedir/foo

		err = system("mv foo bar"); //
		err = fstat(fd, &buf);      // 0; still ok! st_ino unchanged, buf.st_ctim modified
		err = system("ls -lF /proc/self/fd/3"); // 0
		// lr-x------ 1 wsh wsh 64  5月  9 19:14 /proc/self/fd/3 -> /home/wsh/src/apue.3e-clion/filedir/bar

		close(fd);
		err = system("rm bar");
		err = 0;
	}

	exit(0);
}

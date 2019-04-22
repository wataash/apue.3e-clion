#include "apue.h"
#include <termios.h>

int
main(void)
{
	struct termios	term;

	if (tcgetattr(STDIN_FILENO, &term) < 0)
		err_sys("tcgetattr error");

	// c_cflag
	(void)CBAUD; (void)B0; (void)B50; (void)B38400;
	(void)cfgetispeed; (void)cfsetispeed; (void)cfgetospeed; (void)cfsetospeed;
	(void)CSIZE; (void)CS5; (void)CS6; (void)CS7; (void)CS8;
	(void)CSTOPB;
	(void)CREAD;
	(void)PARENB;
	(void)PARODD;
	(void)HUPCL;
	(void)CLOCAL;
	// (void)LOBLK;
	(void)(CBAUDEX|CBAUD); (void)B57600; (void)B115200; (void)B230400; (void)B4000000; (void)__MAX_BAUD;
	(void)CIBAUD;
	(void)CMSPAR;
	(void)CRTSCTS;

	switch (term.c_cflag & CSIZE) {
	case CS5:
		printf("5 bits/byte\n");
		break;
	case CS6:
		printf("6 bits/byte\n");
		break;
	case CS7:
		printf("7 bits/byte\n");
		break;
	case CS8:
		printf("8 bits/byte\n");
		break;
	default:
		printf("unknown bits/byte\n");
	}

	term.c_cflag &= ~CSIZE;		/* zero out the bits */
	term.c_cflag |= CS8;		/* set 8 bits/byte */
	if (tcsetattr(STDIN_FILENO, TCSANOW, &term) < 0)
		err_sys("tcsetattr error");

	exit(0);
}

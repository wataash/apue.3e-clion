#include "apue.h"
#include <termios.h>

int
main(void)
{
	struct termios	term;
	long			vdisable;

	if (isatty(STDIN_FILENO) == 0)
		err_quit("standard input is not a terminal device");

	if ((vdisable = fpathconf(STDIN_FILENO, _PC_VDISABLE)) < 0)
		err_quit("fpathconf error or _POSIX_VDISABLE not in effect");

	if (tcgetattr(STDIN_FILENO, &term) < 0)	/* fetch tty state */
		err_sys("tcgetattr error");

	term.c_cc[VINTR] = vdisable;	/* disable INTR character */
	term.c_cc[VEOF]  = 2;			/* EOF is Control-B */

	// for TCSetAttr()
	(void)TCSANOW;
	(void)TCSADRAIN;
	(void)TCSAFLUSH;

	// Figure 18.9
	cc_t cc;
	cc = 0;                   (term.c_iflag & ICANON);       // \r
	cc = term.c_cc[VDISCARD]; (term.c_lflag & ICANON);       // ^O
#ifdef VDSUSP
	cc = term.c_cc[VDSUSP];   (term.c_lflag & ISIG);         // ^Y
#endif
	cc = term.c_cc[VEOF];     (term.c_lflag & ICANON);       // ^D
	cc = term.c_cc[VEOL];     (term.c_lflag & ICANON);       //
	cc = term.c_cc[VEOL2];    (term.c_lflag & ICANON);       //
	cc = term.c_cc[VERASE];   (term.c_lflag & ICANON);       // ^H, ^?
#ifdef VERASE2
	cc = term.c_cc[VERASE2];  (term.c_lflag & ICANON);       // ^H, ^?
#endif
	cc = term.c_cc[VINTR];    (term.c_lflag & ISIG);         // ^?, ^C
	cc = term.c_cc[VKILL];    (term.c_lflag & ICANON);       // ^U
	cc = term.c_cc[VLNEXT];   (term.c_lflag & IEXTEN);       // ^V
	cc = 0;                   (term.c_lflag & ICANON);       // \n
	cc = term.c_cc[VQUIT];    (term.c_lflag & ISIG);         // ^(backslash)
	cc = term.c_cc[VREPRINT]; (term.c_lflag & ICANON);       // \R
	cc = term.c_cc[VSTART];   (term.c_lflag & IXON & IXOFF); // \Q
#ifdef VERASE2
	cc = term.c_cc[VSTATUS];  (term.c_lflag & ICANON);       // \T
#endif
	cc = term.c_cc[VSTOP];    (term.c_lflag & IXON & IXOFF); // ^S
	cc = term.c_cc[VSUSP];    (term.c_lflag & ISIG);         // ^Z SIGTSTP
	cc = term.c_cc[VWERASE];  (term.c_lflag & ICANON);       // ^W

	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &term) < 0)
		err_sys("tcsetattr error");

	exit(0);
}

#include "apue.h"
#include <sys/socket.h>
#include <netinet/in.h>

#define MAXSLEEP 128

int
connect_retry(int sockfd, const struct sockaddr *addr, socklen_t alen)
{
	int numsec;

	/*
	 * Try to connect with exponential backoff.
	 */
	for (numsec = 1; numsec <= MAXSLEEP; numsec <<= 1) {
		if (connect(sockfd, addr, alen) == 0) {
			/*
			 * Connection accepted.
			 */
			return(0);
		}

		/*
		 * Delay before trying again.
		 */
		if (numsec <= MAXSLEEP/2)
			sleep(numsec);
	}
	return(-1);
}

int
main(int argc, char *argv[])
{
	int sfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sfd == -1) {
	}
	struct sockaddr_in sin = {
	    .sin_addr.s_addr = htonl(0x7f000001), // 127.0.0.1
	    .sin_port = htons(22),                // :22
	};
	connect_retry(sfd, (struct sockaddr *)&sin, sizeof(sin));
}
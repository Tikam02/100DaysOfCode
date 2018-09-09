#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main (int argc, char *argv[]) {
	struct addrinfo hints, *res, *p;
	int status;
	char ipstr[INET6_ADDRSTRLEN];

	if (argc != 2) {
		fprintf (stderr, "Usage: %s <hostname>\n", argv[0]);
		return EXIT_FAILURE;
	}

	memset (&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	status = getaddrinfo (argv[1], NULL, &hints, &res);
	if (status != 0) {
		fprintf (stderr, "getaddrinfo: %s\n", gai_strerror (status));
		return EXIT_FAILURE;
	}

	p = res;
	for (p = res; p; p = p->ai_next) {
		void *addr;
		struct sockaddr_in *ipv4;
		struct sockaddr_in6 *ipv6;

		switch (p->ai_family) {

		/* IPv4 */
		case AF_INET:
			ipv4 = (struct sockaddr_in *) p->ai_addr;
			addr = &ipv4->sin_addr;
			break;

		/* IPv6 */
		case AF_INET6:
			ipv6 = (struct sockaddr_in6 *) p->ai_addr;
			addr = &ipv6->sin6_addr;
			break;

		default:
			fprintf (stderr, "Error: Unknown IP format\n");
			continue;
		}

		inet_ntop (p->ai_family, addr, ipstr, sizeof ipstr);
		printf ("%s\n", ipstr);
	}

	freeaddrinfo (res);

	return EXIT_SUCCESS;
}

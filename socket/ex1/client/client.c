#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>

#define PORT 0x0da2
#define IP_ADDR 0x7f000001

int main(void)
{
	int r;
	int sock = socket(AF_INET, SOCK_STREAM, 0), nrecv;
	struct sockaddr_in s = {0};
	s.sin_family = AF_INET;
	s.sin_port = htons(PORT);
	s.sin_addr.s_addr = htonl(IP_ADDR);
	if (connect(sock, (struct sockaddr*)&s, sizeof(s)) < 0)
	{
		perror("connect");
		return 1;
	}
	printf("Successfully connected.\n");
	char someBuffer[12];
	if ((nrecv = recv(sock, &r, sizeof(r), 0)) < 0)
	{
		perror("recv");
		return 1;
	}
	printf("Successfully received %d bytes. Message Received %d\n", nrecv, r);
	return 0;
}

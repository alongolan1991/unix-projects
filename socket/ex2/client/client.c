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
	char file_name[] = "hello world.txt";
	int file_name_len;
	long file_real_size;
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

	file_name_len = strlen(file_name);
	printf("file name len %d\n: ",sizeof(file_name_len));
	if (send(sock, &file_name_len, sizeof(file_name_len), 0) < 0)
	{
		perror("send1");
		return 1;
	}
	printf("file name len %d\n: ",file_name_len);
	if (send(sock, file_name, file_name_len, 0) < 0)
	{
		perror("send2");
		return 1;
	}


	printf("file_real_size %d\n: ",sizeof(file_real_size));
	if (recv(sock, &file_real_size, sizeof(file_real_size), 0) < 0)
	{
		perror("recive");
		return 1;
	}
	printf("the file contain %d bytes\n", file_real_size);
	char buffer[file_real_size + 1];
	memset(buffer, 0, sizeof(buffer));
	printf("size of buffer= %d\n",sizeof(buffer));
	if (recv(sock, buffer, sizeof(buffer), 0) < 0)
	{
		perror("recive");
		return 1;
	}


	printf("file content:%s\n", buffer);

	return 0;
}

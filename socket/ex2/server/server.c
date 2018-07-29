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
#include <fcntl.h>

#define PORT 0x0da2
#define IP_ADDR 0x7f000001
#define QUEUE_LEN 20

int main(void)
{
	int n_read,n_write,i;
	long file_size;
	int name_len;
	int listenS = socket(AF_INET, SOCK_STREAM, 0);
	if (listenS < 0)
	{
		perror("socket");
		return 1;
	}
	struct sockaddr_in s = {0};
	s.sin_family = AF_INET;
	s.sin_port = htons(PORT);
	s.sin_addr.s_addr = htonl(IP_ADDR);
	if (bind(listenS, (struct sockaddr*)&s, sizeof(s)) < 0)
	{
		perror("bind");
		return 1;
	}
	if (listen(listenS, QUEUE_LEN) < 0)
	{
		perror("listen");
		return 1;
	}
	struct sockaddr_in clientIn;
	int clientInSize = sizeof clientIn;
	int newfd = accept(listenS, (struct sockaddr*)&clientIn, (socklen_t*)&clientInSize);
	if (newfd < 0)
	{
		perror("accept");
		return 1;
	}
	printf("len of int = %d\n", sizeof(name_len));
	if (recv(newfd, &name_len, sizeof(name_len), 0) < 0)
	{
		perror("recive");
		return 1;
	}
	char name[name_len + 1];
	memset(name, 0 , sizeof(name));
	printf("name len = %d\n", name_len);
	if (recv(newfd, name, name_len, 0) < 0)
	{
		perror("recive");
		return 1;
	}
	// printf("file name :%s\n", name);
	int newFile = open(name, O_RDONLY);
	if(newFile < 0){
		perror("open");
		return 1;
	}

	struct stat st;
	fstat(newFile, &st);
	file_size = st.st_size;
	printf("st_size = %d\n",sizeof(file_size));
if(send(newfd, &file_size, sizeof(file_size), 0) < 0)
	{
		perror("send1");
		return 1;
	}

	char buf[file_size + 1];
	memset(buf, 0 , sizeof(buf));
	n_read = 1;
	while(n_read != 0){

		n_read =	read(newFile, buf , sizeof(buf));
		for(i=0; i < n_read; i += n_write){
			printf("size of buffer: %d\n",n_read - i);
			send(newfd, buf + i, n_read - i , 0);
		}
	}


	printf("Done sending data to client %d. Closing socket.\n", newfd);
	close(newfd);
	close(listenS);
	return 0;
}

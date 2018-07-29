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

int main(int argc, char *argv[])
{
	if(argc != 3 ){
		printf("file need cotatin 2 arguments\n %d", argc);
		return 1;
	}

	char file_name[20];
	char action[14];
	int file_name_len;
	int uid;
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
	memset(file_name, 0 , sizeof(file_name));
	memset(action, 0 , sizeof(action));
	strcpy(action,argv[1]);
	strcpy ( file_name, argv[2]);

	file_name_len = strlen(file_name);
	if (send(sock, &file_name_len, sizeof(file_name_len), 0) < 0)
	{
		perror("send1");
		return 1;
	}
	if (send(sock, file_name, file_name_len, 0) < 0)
	{
		perror("send2");
		return 1;
	}
	if (send(sock, action, sizeof(action), 0) < 0)
	{
		perror("send2");
		return 1;
	}

	if (recv(sock, &file_real_size, sizeof(file_real_size), 0) < 0)
	{
		perror("recive");
		return 1;
	}



	if (strcmp(action,"get-file-info") == 0){
	if (recv(sock, &uid, sizeof(uid), 0) < 0)
	{
		perror("recive");
		return 1;
	}
	printf("%s - size : %ld bytes and the owner id is : %ld\n", file_name, file_real_size, uid );
		close(sock);
		return 0;
	}


	else if (strcmp(action,"download-file") == 0){
		int count = 0;
		int total = 0;
	int k;
	char buffer[file_real_size ];
	memset(buffer, 0, sizeof(buffer));

	while ((count = recv(sock, &buffer[total], sizeof buffer - count, 0)) > 0)
	{

	    total += count;

	}
	if (count == -1)
	{
	    perror("recv");
	}

	if(open(file_name,O_EXCL) != -1){
		printf("File does exists\n");
	}
		else{
			int new_File = open(file_name, O_RDWR|O_CREAT, 0666);
			if(new_File < 0){
				perror("open");
				return 1;
			}

				write(new_File, buffer, sizeof(buffer));
				printf("the file added to your directory\n");
				close(new_File);
		}

}

else{
	printf("COMMAND ERROR: you can use 'get-file-info' or 'download-file'\n");

}

	return 0;
}

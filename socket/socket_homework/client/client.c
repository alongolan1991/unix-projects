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

	int file_exists;
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
	// send the length of the file name
	if (send(sock, &file_name_len, sizeof(file_name_len), 0) < 0)
	{
		perror("send1");
		return 1;
	}
	// send the name of the file
	if (send(sock, file_name, file_name_len, 0) < 0)
	{
		perror("send2");
		return 1;
	}
	// send the command "get-file-info" or "download-file"
	if (send(sock, action, sizeof(action), 0) < 0)
	{
		perror("send2");
		return 1;
	}
	// get from the server message if the file exist
	if (recv(sock, &file_exists, sizeof(file_exists), 0) < 0)
	{
		perror("recive");
		return 1;
	}

	if(file_exists == 1){
		printf("%s does not exist in the server\n", file_name);
		return 0;
	}

	// get the file size
	if (recv(sock, &file_real_size, sizeof(file_real_size), 0) < 0)
	{
		perror("recive");
		return 1;
	}
	// if the action "get-file-info" get the user id
	if (strcmp(action,"get-file-info") == 0){
	if (recv(sock, &uid, sizeof(uid), 0) < 0)
	{
		perror("recive");
		return 1;
	}
	printf("%s - size : %ld bytes and the owner ID is : %d\n", file_name, file_real_size, uid );
		// print the file details and close connection
		close(sock);
		return 0;
	}

	// if the action "download-file" get the file
	else if (strcmp(action,"download-file") == 0){
		int count = 0;
		int total = 0;
	int k;

	char* buffer = (char*) malloc(file_real_size);
	memset(buffer, 0, file_real_size);

	while ((count = recv(sock, buffer + total, file_real_size - total, 0)) > 0)
	{
	    total += count;
	}
	if (count == -1)
	{
	    perror("recv");
	}
	// check if the file alreay exist
	if(open(file_name,O_EXCL) != -1){
		printf(" %s alreay exists\n", file_name);
	}
		else{
			// open the file for write
			int new_File = open(file_name, O_RDWR|O_CREAT, 0666);
			if(new_File < 0){
				perror("open");
				return 1;
			}
				// write the file the information
				write(new_File, buffer, file_real_size);
				printf("the file added to your directory\n");
				close(new_File);
		}
free(buffer);
}

else{
	printf("COMMAND ERROR: you can use 'get-file-info' or 'download-file'\n");

}

	return 0;
}

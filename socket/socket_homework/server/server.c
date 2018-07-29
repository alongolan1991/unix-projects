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
	int pid;
	char action[14];
	char file_path[20];
	char temp[20];
	int n_read,n_write,i;
	long file_size;
	int name_len;
	int new_File;
	struct stat st;
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
	for(;;){
	int newfd = accept(listenS, (struct sockaddr*)&clientIn, (socklen_t*)&clientInSize);
	if (newfd < 0)
	{
		perror("accept");
		return 1;
	}
	if((pid = fork()) == -1){
		close(newfd);
		continue;
	}
	else if(pid > 0)
			 {
					 close(newfd);
					 continue;
			 }

	else if(pid == 0){
		if (recv(newfd, &name_len, sizeof(name_len), 0) < 0)
		{
			perror("recive");
			return 1;
		}
		char name[name_len + 1];
		memset(name, 0 , sizeof(name));
		memset(temp, 0, sizeof(temp));
		if (recv(newfd, name, name_len, 0) < 0)
		{
			perror("recive");
			return 1;
		}
			memset(action, 0 , sizeof(action));
		if (recv(newfd, action, sizeof(action), 0) < 0)
		{
			perror("recive");
			return 1;
		}
		 strcpy ( temp, name);

			memset(file_path, 0 , sizeof(file_path));
			strcat (file_path,"disc/");
			strcat (file_path,temp);

					new_File = open(file_path, O_RDONLY);
			if(new_File < 0){
				perror("open");
				return 1;
			}
			fstat(new_File, &st);
			file_size = st.st_size;

			if(send(newfd, &file_size, sizeof(file_size), 0) < 0)
				{
					perror("send1");
					return 1;
				}

				if (strcmp(action,"get-file-info") == 0){
				if(send(newfd, &st.st_uid, sizeof(st.st_uid), 0) < 0)
					{
						perror("send1");
						return 1;
					}
				}

				else if(strcmp(action,"download-file") == 0){
					char buf[file_size + 1];
					memset(buf, 0 , sizeof(buf));
					n_read = 1;
					int k;
					while(n_read != 0){
						n_read =	read(new_File, buf , sizeof(buf));
						for(i=0; i < n_read; i += n_write){
						n_write =	send(newfd, buf + i, n_read - i , 0);
						}
					}
				}
				close(newfd);
				continue;
	}

	}


	close(listenS);
	return 0;
}

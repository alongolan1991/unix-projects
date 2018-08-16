#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>


int main(int argc, char* argv[]){
  int file_to_read,file_size,num_of_commands;
  int i,j,k;
  int flag;
  int fd[2];
  int new_File;
  char buf[4096] = {0};
  char my_func[100][20] = {0};
  char* my_func1 [100][5] = {0};
  char integer_string[32];
  char* file_na1;
  char* token;
  pid_t* child_pid;
  int* ret_array;

  if(argc != 2){
    printf("Put your function file\n");
    return 1;
  }
  // open the commands file
  file_to_read = open(argv[1], O_RDONLY);
  if(file_to_read < 0){
    perror("open");
    return 1;
  }
    // read from the file to local array
  	if((read(file_to_read, buf , sizeof(buf))) == -1){
      perror("read");
      return 1;
    }
      // copy and separat commands to local matrix
      num_of_commands = 0;
      token = strtok(buf, "\n");
      while( token != NULL ) {

      strcpy(my_func[num_of_commands], token);
      token = strtok(NULL, "\n");
      num_of_commands++;
   }
   // copy and separat words
   for(i=0; i < num_of_commands; i++){
     token = strtok(my_func[i]," ");
     while( token != NULL){
       my_func1[i][j] =(char*) malloc(strlen(token) + 1);
       strcpy(my_func1[i][j],token);
       token = strtok(NULL," ");
       j++;
     }
     j=0;
   }
      // creating array of pid and array of return value
      child_pid = (pid_t*)malloc(sizeof(pid_t) * num_of_commands);
      ret_array =(int*)malloc(sizeof(int) * num_of_commands);

      pipe(fd);
      flag = fcntl(fd[0], F_GETFL);
      flag |= O_NONBLOCK;
      fcntl(fd[0], F_SETFL, flag);
      close(STDERR_FILENO);

     for(i=0; i<num_of_commands; i++){
       sprintf(integer_string, "OUT%d", i);
       new_File = open(integer_string, O_RDWR| O_TRUNC | O_CREAT, 0666);
       if(new_File < 0){
         perror("open");
         return 1;
       }
     child_pid[i] = fork();
     if(child_pid[i] == 0) {
    // change the STDOUT to the pipe
      dup2(fd[1],STDOUT_FILENO);
      close(fd[0]);
    if(execvp(my_func1[i][0], my_func1[i]) == -1 )
    {
       perror("exevp");
     }
    /* If execvp returns, it must have failed. */
    printf("Unknown command\n");
    return 0;
  }
  if(child_pid[i] > 0){
    close(fd[1]);
    int num_of_char = 0;
    memset(buf,0,sizeof(buf));
    for(k=0; k < 10000; k++){
      if((num_of_char = read(fd[0],buf, sizeof(buf))) != -1)
        break;
    }
    write(new_File, buf, num_of_char);
  }
   }
   for(i = 0 ; i < num_of_commands ; i++){
      waitpid(child_pid[i], &ret_array[i], 0);
   }

   // print the details about the child process
   for(i=0 ; i < num_of_commands ; i++){
      while(my_func1[i][j] != NULL ){
        printf("%s " ,my_func1[i][j]);
        free(my_func1[i][j]);
        j++;
      }
      printf(": %d\n",WEXITSTATUS(ret_array[i]));
      j=0;
   }
   free(child_pid);
   free(ret_array);
   close(file_to_read);
  return 0;
}

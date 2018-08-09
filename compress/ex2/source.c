#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>

int main (){
pid_t pidArray[10];
int *sharedArray;
int i,j,k;

sharedArray =(int*) mmap(NULL, sizeof(int) * 10, PROT_READ | PROT_WRITE , MAP_SHARED | MAP_ANONYMOUS, -1 , 0);
memset(sharedArray, 0 , sizeof(int) * 10);
for(i = 0; i < 10; i++){
  if((pidArray[i] = fork()) == -1){
    printf("fork error");
  }
  if(pidArray[i] == 0){
    sharedArray[i]++;
    exit(1);
  }
}
  for(j = 0 ; j < 10 ; j++){
    waitpid(pidArray[j], NULL, 0);
  }
  for(k = 0 ; k < 10 ; k++)
    printf(" %d", sharedArray[k]);
    printf("\n");

  return 0;
}

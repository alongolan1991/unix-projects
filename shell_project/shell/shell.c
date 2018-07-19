#include <stdio.h>
#include <unistd.h>
#include <dirent.h>

int main(){

  DIR* Dir;
  struct dirent* ent;
  char command[10];
  char cwd[1024];
  while(1){
    printf(">");
    scanf("%s",&command);
    if(strcmp("ls",command) == 0){
      getcwd(cwd , sizeof(cwd));
    if((Dir = opendir(cwd)) == NULL)
      printf("cant open direcory");
    while((ent = readdir(Dir)) != NULL)
        printf("%s\n",ent->d_name);
      }
    else if(strcmp("cd",&command) == 0){
      scanf("%s",&command);
      chdir(command);
    }
    else{
      system(command);
    }
  }
return 0;

}

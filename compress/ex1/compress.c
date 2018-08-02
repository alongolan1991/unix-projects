#include <stdio.h>
#include <fcntl.h>

int main (int argc, const char* argv[])
{
int fw = open(argv[1],O_RDWR | O_CREAT , 0600);
int ret;
if ((ret = fork()) > 0)
close(ret);
else if (ret == 0){
  dup2(fw,1);
  execl("/bin/gzip", "/bin/gzip", NULL);
}
else
perror("fork");
printf("End of the road.\n");
return 0;
}

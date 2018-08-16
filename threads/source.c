#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

pthread_t my_thread[4];

typedef struct range{
  int start;
  int end;
  int len;
  int* myarray;
} range;


void * findLarge(void* s){
  int large,i;
  range* temp = (struct range*)s;
  large = temp->myarray[0];
  for(i = 1; i < temp->len ; i++ ){
    if(large < temp->myarray[i])
      large = temp->myarray[i];
    }
    return (void*)large;
}



int main( int argc, char **argv ){
  void* max[4];
range myRange[4];
int i,biggest;
int* myArray;
myArray = (int*) malloc(sizeof(int)* (argc -1));
for(i = 0 ; i < argc -1 ; i++)
  myArray[i] = atoi(argv[i +1]);
int j=0;
for(i = 0 ; i < 4; i++){
  myRange[i].start = j;
  myRange[i].end =  j + ((argc - 1) / 4);
  myRange[i].len = (argc-1) / 4;
  myRange[i].myarray = myArray + j;
  j+=(argc-1) / 4;
}

for(i= 0; i< 4 ; i++){
  pthread_create(&my_thread[i], NULL, &findLarge, &myRange[i]);
}
for( i = 0 ; i < 4 ; i ++){
  pthread_join(my_thread[i], &max[i]);
}
biggest = (int)max[0];
for(i = 1 ; i < 4 ; i++)
{
    if(biggest < max[i])
      biggest = (int)max[i];

}
printf("the max is: %d\n",biggest);
  return 0;
}

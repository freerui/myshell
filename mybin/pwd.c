#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main()
{
   char path[128]={0};
   getcwd(path,127);
   printf("%s\n",path);

   exit(0);
}

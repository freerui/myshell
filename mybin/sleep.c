#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main(int argc,char *argv[])
{
   int time=0;
   sscanf(argv[1],"%d",&time);
   sleep(time);

   exit(0);
}

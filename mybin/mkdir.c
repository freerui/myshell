#include <sys/stat.h> 
#include <sys/types.h> 
#include <stdio.h> 
 
 
int main(int argc, char *argv[])
{ 
 
  int rt; 
  int i;
  if(argc<2)
  {
	  printf("mkdir: missing operand\nTry `mkdir --help' for more information.\n");
  }
  for(i=1;i<argc;i++)
  {
 	 if( (rt = mkdir (argv[i],10705)) == -1 )
	 { 
    	 printf("mkdir: cannot create directory `%s': File exists\n",argv[i]); 
 	 } 
  }
 
  return 0; 
 
}

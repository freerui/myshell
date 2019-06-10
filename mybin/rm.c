#include <unistd.h> 
#include <stdio.h> 
#include <stdlib.h> 
 
int main(int argc , char * argv[])
{ 
 
 int rt; 
 int i;
 if(argc <2)
 { 
    printf("rm: missing operand\nTry `rm --help' for more information.\n");
 }
 else
 { 
 
   for(i=1;i<argc;i++)
   {
  	 if((rt = unlink(argv[i])) !=  0)
 	  { 
      	 printf("rm: cannot remove `%s': No such file or directory\n",argv[i]); 
   	   }
   } 
    return 0;
 
 } 
   
 return 0; 
 
}

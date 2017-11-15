#include <stdio.h>

int  main(int argc, char* argv[])
{
	if(argc!=2)
	{
		printf("pi_main rip");
	}
 
	printf("remote:%s", argv[1]);
  
	return 0;
}

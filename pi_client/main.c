#include <stdio.h>
#include "state_client.h"
#include "wav_client.h"
#include "wav_server.h"
#include <stdlib.h>
#include <string.h>
#include<unistd.h>

static char rip[32] = {"192.168.100.2\0"};
static int rport = 1201;
static int lport = 1201;
int state_client_callback(int cmd, int para)
{
	printf("%s cmd=%d, para=%d\r\n",__FUNCTION__, cmd, para);
	switch(cmd)
	{
	case 1:
		wav_server_start(lport);
		wav_client_start(rip, rport);

		break;
	case 0:
		wav_client_stop();
		wav_server_stop();
		break;
	default:
		break;
	}
	
	return 0;
}

int  main(int argc, char* argv[])
{
	if(argc!=4)
	{
		printf("pi_main rip rport lport\r\n");
		return -1;
	}
	memset(rip,0,32);
 	strcpy(rip,argv[1]);
	rport = atoi(argv[2]);
	lport = atoi(argv[3]);
	printf("remote:%s:%d,  local = %d\r\n", rip, rport, lport);
	
	state_client_start(state_client_callback, rip, rport);
	while(1){
		sleep(1);	
	}
	return 0;
}

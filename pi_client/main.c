#include <stdio.h>
#include "state_client.h"
#include "wav_client.h"
#include "wav_server.h"
#include <stdlib.h>
#include <string.h>
#include<unistd.h>
#include "log4z.h"  
  using namespace zsummer::log4z;  
static char rip[32] = {"127.0.0.1\0"};
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
//start log4z  
      ILog4zManager::getRef().start();  
 //begin test format log input....  
      LOGFMTT("format input *** %s *** %d ***", "LOGFMTT", 123456);  
      LOGFMTD("format input *** %s *** %d ***", "LOGFMTD", 123456);  
      LOGFMTI("format input *** %s *** %d ***", "LOGFMTI", 123456);  
      LOGFMTW("format input *** %s *** %d ***", "LOGFMTW", 123456);  
      LOGFMTE("format input *** %s *** %d ***", "LOGFMTE", 123456);  
      LOGFMTA("format input *** %s *** %d ***", "LOGFMTA", 123456);  
      LOGFMTF("format input *** %s *** %d ***", "LOGFMTF", 123456);  
 
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
	LOGA("main quit ...");  
	return 0;
}

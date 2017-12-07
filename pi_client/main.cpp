#include <stdio.h>
#include "state_client.h"
#include "wav_client.h"
#include "wav_server.h"
#include "broadcast_server.h"
#include <stdlib.h>
#include <string.h>
#include<unistd.h>
#include "log4z.h"  
  using namespace zsummer::log4z;  
static char rip[32] = {"127.0.0.1\0"};
static int rport = 1201;
static int lport = 1201;
static int device_id = 0;
int state_client_callback(int cmd, int para)
{
	LOGFMTT("%s cmd=%d, para=%d",__FUNCTION__, cmd, para);
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
	ILog4zManager::getPtr()->setLoggerLevel(LOG4Z_MAIN_LOGGER_ID, LOG_LEVEL_TRACE);
 //begin test format log input....  
      LOGFMTT("format input *** %s *** %d ***", "LOGFMTT", 123456);  
      LOGFMTD("format input *** %s *** %d ***", "LOGFMTD", 123456);  
      LOGFMTI("format input *** %s *** %d ***", "LOGFMTI", 123456);  
      LOGFMTW("format input *** %s *** %d ***", "LOGFMTW", 123456);  
      LOGFMTE("format input *** %s *** %d ***", "LOGFMTE", 123456);  
      LOGFMTA("format input *** %s *** %d ***", "LOGFMTA", 123456);  
      LOGFMTF("format input *** %s *** %d ***", "LOGFMTF", 123456);  
 
	if(argc!=5)
	{
		LOGFMTI("pi_main rip rport lport dev_id");
		return -1;
	}
	memset(rip,0,32);
 	strcpy(rip,argv[1]);
	rport = atoi(argv[2]);
	lport = atoi(argv[3]);
	device_id  = atoi(argv[4]);
	LOGFMTT("remote:%s:%d,  local = %d, dev_id=%d", rip, rport, lport, device_id);
	
	state_client_start(state_client_callback, rip, rport);
	broadcast_server_start(1200,device_id );
	while(1){
		sleep(1);	
	}
	LOGA("main quit ...");  
	return 0;
}

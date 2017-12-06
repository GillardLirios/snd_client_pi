#include "broadcast_server.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include<unistd.h>
#include <pthread.h>
#include "rpi_gpio.h"
#include <event.h>
#include <string.h>  
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <arpa/inet.h>  
#include <netdb.h>  
#include "pcm_io.h"
#include "log4z.h"
int broadcast_server_exit = 0;
void* broadcast_server_thread(void *ptr);
int broadcast_local_port = 1200;
int broadcast_server_start(int port)
{
	LOGFMTT("%s %d",__FUNCTION__, port);
	broadcast_server_exit = 0;
	if(port)
		broadcast_local_port = port;
	pthread_t fd_thread_server;
	pthread_attr_t attr;
	pthread_attr_init (&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	char message1[1024] = "thread broadcast server";
	int ret_thrd;
	ret_thrd = pthread_create(&fd_thread_server, &attr, broadcast_server_thread, (void *) message1);
	pthread_attr_destroy (&attr);
	if (ret_thrd != 0) {
         LOGFMTT("线程%s创建失败",message1);
     } else {
         LOGFMTT("线程%s创建成功",message1);
	}
	return 0;
}

void broadcast_server_stop(void)
{
	LOGFMTT("%s", __FUNCTION__);
	broadcast_server_exit = 1;
//	gpio_write(19,1); // close pa
}



void* broadcast_server_thread(void *ptr)
{
	LOGFMTT("%s, %s start", __FUNCTION__, (char*)ptr);
	//gpio_direction(19,OUT);	//3110 en
	//gpio_write(19,0); // open pa

	 unsigned  int sin_len;  
    	char message[10240];  
  	char* ppcm = message;
	int socket_descriptor;  
	struct sockaddr_in sin;  
	//printf("Waiting for data form sender ");  

	bzero(&sin,sizeof(sin));  
	sin.sin_family=AF_INET;  
	sin.sin_addr.s_addr=htonl(INADDR_ANY);  
	sin.sin_port=htons(broadcast_local_port );  
	sin_len=sizeof(sin);  

	socket_descriptor=socket(AF_INET,SOCK_DGRAM,0);  
	if(-1 ==socket_descriptor)
	{
		LOGFMTE("%s, socket fd is null %d", __FUNCTION__, errno);
	}
	if(-1==bind(socket_descriptor,(struct sockaddr *)&sin,sizeof(sin)))
	{
		LOGFMTE("%s, bind err %d", __FUNCTION__, errno);
	}
	pcm_out_init2();
	LOGFMTT("%s,   looping", __FUNCTION__);
	while(1)
	{
		if(broadcast_server_exit)
		{
			LOGFMTT("%s, %s break", __FUNCTION__, (char*)ptr);
			break;
		}
		int ret = recvfrom(socket_descriptor,message,sizeof(message),0,(struct sockaddr *)&sin,&sin_len);  
        	if(1314==ret)
		{
//			LOGFMTT("%s, %ld, rcv:%d\n", __FUNCTION__, clock(), ret);  
			unsigned int u32sample = 0;
			int i32sample = 0;
			short i16sample = 0;
			#if 1
			for(int i=34;i<1314;)
			{
				u32sample = (message[i] | message[i+1]<<8);
				//printf("%X\t", u32sample);
				i32sample = u32sample;
				i32sample = (i32sample -32768);
				//printf("%X\t",i32sample);
				i16sample = (short)i32sample;
				//printf("%d\t",i16sample);
				message[i] = i16sample & 0xff;
				//printf("%X\t",message[i] );
				message[i+1] = (i16sample>>8)&0xff;
				//printf("%X\n",message[i+1] );
				message[i+2] = message[i] ;
				message[i+3] = message[i+1] ;
				i+=4;
			}
			#endif
			//write_frames2(ppcm, ret);
			write_frames2(ppcm+34, 1280);
		}
		else
		{
			usleep(3000);
		}
	}
//	gpio_write(19,1); // close pa
//	gpio_unexport(6);
//	gpio_unexport(13);
//	gpio_unexport(19);
    	close(socket_descriptor);  
	LOGFMTT("%s, %s end", __FUNCTION__, (char*)ptr);
	return 0;
}



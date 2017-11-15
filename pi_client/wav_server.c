#include "wav_server.h"
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
int server_exit = 0;
void* wav_server_thread(void *ptr);
int local_port = 1202;
int wav_server_start(int port)
{
	printf("%s %d\r\n",__FUNCTION__, port);
	server_exit = 0;
	if(port)
		local_port = port;
	pthread_t fd_thread_server;
	pthread_attr_t attr;
	pthread_attr_init (&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	char message1[1024] = "thread wav server";
	int ret_thrd;
	ret_thrd = pthread_create(&fd_thread_server, &attr, wav_server_thread, (void *) message1);
	pthread_attr_destroy (&attr);
	if (ret_thrd != 0) {
         printf("线程%s创建失败\n",message1);
     } else {
         printf("线程%s创建成功\n",message1);
	}
	return 0;
}

void wav_server_stop(void)
{
	printf("%s\n", __FUNCTION__);
	server_exit = 1;
	gpio_write(19,1); // close pa
}



void* wav_server_thread(void *ptr)
{
	printf("%s, %s start\n", __FUNCTION__, (char*)ptr);
	gpio_direction(19,OUT);	//3110 en
	gpio_write(19,0); // open pa

	 unsigned  int sin_len;  
    char message[1024000];  
  
    int socket_descriptor;  
    struct sockaddr_in sin;  
    printf("Waiting for data form sender \n");  
  
    bzero(&sin,sizeof(sin));  
    sin.sin_family=AF_INET;  
    sin.sin_addr.s_addr=htonl(INADDR_ANY);  
    sin.sin_port=htons(local_port );  
    sin_len=sizeof(sin);  
  
    socket_descriptor=socket(AF_INET,SOCK_DGRAM,0);  
    bind(socket_descriptor,(struct sockaddr *)&sin,sizeof(sin));  
	pcm_out_init();
	while(1)
	{
		if(server_exit)
		{
			break;
		}
		int ret = recvfrom(socket_descriptor,message,sizeof(message),0,(struct sockaddr *)&sin,&sin_len);  
        if(ret)
		{
			printf("%s, %ld, rcv:%d\n", __FUNCTION__, clock(), ret);  
			write_frames(message, ret);
		}
		else
		{
			usleep(3000);
		}
	}
	gpio_write(19,1); // close pa
//	gpio_unexport(6);
//	gpio_unexport(13);
//	gpio_unexport(19);
    close(socket_descriptor);  
	printf("%s, %s exit\n", __FUNCTION__, (char*)ptr);
	return 0;
}



#include "wav_client.h"
#include <string.h>
#include <stdio.h>
#include "pcm_io.h"
#include <stdlib.h>
#include<unistd.h>
#include <pthread.h>
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <arpa/inet.h>  
#include <netdb.h>
int client_exit = 0;
void wav_client_thread(void *ptr);
char remote_ip[16];
int remote_port = 7000;
int wav_client_start(const char* ip, int port)
{
	printf("%s %s:%d\r\n",__FUNCTION__,ip, port);
	client_exit = 0;
	if(strlen(ip)<16)
		strcpy(remote_ip,ip);
	else
	{
		printf("invalid ip addr %s\n", ip);
	}
	remote_port = port;
	pthread_t fd_thread_client;
	pthread_attr_t attr;
	pthread_attr_init (&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	char *message1 = "thread wav client";
	int ret_thrd;
	ret_thrd = pthread_create(&fd_thread_client, &attr, (void *)&wav_client_thread, (void *) message1);
	pthread_attr_destroy (&attr);
	if (ret_thrd != 0) {
         printf("线程%s创建失败\n",message1);
     } else {
         printf("线程%s创建成功\n",message1);
	}
	return 0;
}

void wav_client_stop(void)
{
	printf("%s\n", __FUNCTION__);
	client_exit = 1;
}



static void* cap_frames;

void wav_client_thread(void *ptr)
{
	printf("%s, %s start\n", __FUNCTION__, (char*)ptr);
	int frame_len = pcm_cap_init();
	cap_frames = malloc(frame_len );
  	if (cap_frames  == NULL) {
		fprintf(stderr, "failed to allocate frames\n");
		pcm_cap_close();
		return ;
 	}
	else
	{
		printf("cap frame size=%d\n",frame_len);
	}

	int socket_descriptor; //套接口描述字  
    char buf[1024000];  
    struct sockaddr_in address;//处理网络通信的地址  
  
    bzero(&address,sizeof(address));  
    address.sin_family=AF_INET;  
    address.sin_addr.s_addr=inet_addr(remote_ip);//这里不一样  
    address.sin_port=htons(remote_port);  
  
    //创建一个 UDP socket  
  
    socket_descriptor=socket(AF_INET,SOCK_DGRAM,0);//IPV4  SOCK_DGRAM 数据报套接字（UDP协议） 
	buf[0] = 0x5a;
	buf[1] = 0;
	memset(buf+2,0xff,32);
	while(1)
	{
		if(client_exit)
		{
			break;
		}
		int size = read_frames(&cap_frames);
		if (size == 0) {
			printf("read frame size 0\n");
		    break;
		}
		else
		{
			//printf("cap %d\n",size);
			int buf_idx = 0;
			char* pf =  (char*)cap_frames;
			for(int i=0;i<size;i+=2)
			{
				buf[buf_idx++] = pf[i];
				buf[buf_idx++] = pf[i+1];
				buf[buf_idx++] = pf[i];
				buf[buf_idx++] = pf[i+1];
			}
			buf[1] ++;
			//memcpy(buf,cap_frames,size);
			 sendto(socket_descriptor,buf,buf_idx,0,(struct sockaddr *)&address,sizeof(address));  
		}
		//sleep(3);
	}
	free(cap_frames);
	pcm_cap_close();
	close(socket_descriptor);  
	printf("%s, %s exit\n", __FUNCTION__, (char*)ptr);

}



#include "state_client.h"
#include <stdio.h>
#include <stdlib.h>
#include	<unistd.h>
#include <unistd.h>
#include <pthread.h>
#include "rpi_gpio.h"
#include <event.h>

void state_thread(void *ptr);
STATE_CLIENT_CALLBACK  cb_fun;
int state_client_start(STATE_CLIENT_CALLBACK cb,const char* ip, int port)
{
	printf("%s %s:%d\r\n",__FUNCTION__,ip, port);
	cb_fun = cb;
	//cb(1, 0);
	pthread_t fd_thread;
	pthread_attr_t attr;
	pthread_attr_init (&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	char *message1 = "thread state client";
	int ret_thrd;
	ret_thrd = pthread_create(&fd_thread, &attr, (void *)&state_thread, (void *) message1);
	pthread_attr_destroy (&attr);
	if (ret_thrd != 0) {
         printf("线程%s创建失败\n",message1);
     } else {
         printf("线程%s创建成功\n",message1);
	}
	return 0;
}
void state_client_stop(void)
{
}
#if 0
typedef struct
{
	int 		id;
	int 		state;
	char 	text[32];
}t_state_machine;

typedef struct
{
	int 				state_trans_id;
	T_STATE 	cur_state;
	T_EVENT	event;
	T_STATE		next_state;
	void		(*pFun)(int);
}T_STATE_TRANS;

typedef enum
{
	S_IDLE	= 0;
	S_CALLING,
	S_CALL_REQUST_TIMEOUT,
	S_CALL_SPEAK_TIMEOUT,
}T_STATE;
typedef enum
{
	E_CALL_BUTTON_PD =0;
	E_CALL_REQUST,
	E_CALL_TIMEOUT,
	E_CALL_HUNGUP
}T_EVENT;
#endif
void state_thread(void *ptr)
{
	printf("%s, %s start\n", __FUNCTION__, (char*)ptr);
	//gpio_export(6);
	//gpio_export(13);
	//gpio_export(19);
	gpio_export(17);
	gpio_direction(6,IN);	//door sensor
	gpio_direction(13,OUT);	//pa pwr en
	gpio_direction(19,OUT);	//3110 en
	gpio_direction(17,IN);	//call , high
	int gpio13 = 0;
	int call_button = 0;  
	int is_calling = 0;
	long int calling_time = 0;
	const long int MAX_CALLING_TIME = 24*3600.3*60*100; // 18 sec

 
  
	while(1)
	{
		call_button = gpio_read(17);
		//printf("gpio17=%d\r\n", gpio_read(17));
		if(!is_calling && call_button)
		{
			is_calling = 1;
			calling_time  = 0;
			printf("call pressed\n");
			cb_fun(is_calling ,0);
		}
		if(is_calling)
		{
			if(calling_time++>MAX_CALLING_TIME)
			{
				printf("calling timeout\n");
				is_calling  = 0;
				cb_fun(is_calling ,0);
			}
		}
		gpio_write(13,gpio13);
		gpio13 = !gpio13;
		usleep(10000);
	}
//	gpio_unexport(6);
//	gpio_unexport(13);
//	gpio_unexport(19);
	printf("%s, %s exit\n", __FUNCTION__, (char*)ptr);
}





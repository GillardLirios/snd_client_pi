#ifndef __STATE_CLIENT_H__
#define __STATE_CLIENT_H__
typedef int (*STATE_CLIENT_CALLBACK)(int cmd, int para);  

int state_client_start(STATE_CLIENT_CALLBACK cb,const char* ip, int port);
void state_client_stop(void);
#endif


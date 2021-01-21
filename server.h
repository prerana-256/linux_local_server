/*header files will be included here*/
#ifndef __SERVER__
#define __SERVER__
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netinet/ip.h>
#include<arpa/inet.h>
#include<ctype.h>
#include<signal.h>
#include<errno.h>
#include<sys/wait.h>
#include<mysql/mysql.h>
#include "log.h"
#include<pthread.h>
#include "DBHandler.h"

#define SERVER_IP		"192.168.1.8"
#define SERVER_PORT 	2611

char user_name[32];
char password[32];

int srv_fd;
struct sockaddr_in srv_addr;

int init_server(void);
void sigchld_handler(int sig);
void sigint_handler(int sig);
int finish_with_error(MYSQL *);
void * client_thread(void *);
void close_all(void);


#endif
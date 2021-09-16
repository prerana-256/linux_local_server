#ifndef __SERVER__
#define __SERVER__

#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <signal.h>
#include <errno.h>
#include <sys/wait.h>
#include <mysql/mysql.h>
#include <json-c/json.h>
#include "log.h"
#include <pthread.h>
#include "DBHandler.h"
#include "user_menu.h"

#define SERVER_IP		"192.168.43.209"
#define SERVER_PORT 	2611

int srv_fd;
struct sockaddr_in srv_addr;

int init_server(void);
void sigchld_handler(int sig);
void sigint_handler(int sig);
int finish_with_error(MYSQL *);
void * client_thread(void *);
void close_all(void);

#endif
		/*server init, interrupt handling,client thread creation,close connections*/

#include "server.h"

/*child interrupts handling*/
void sigchld_handler(int sig)
{
	int s;
	waitpid(-1, &s, 0);
	
}

/*	
	operation on the client message 
	converts the lower case data to upper case 
*/
void strupr(char *s)
{
	while(*s != '\0')
	{
			if(islower(*s))
					*s=*s-32;
			s++;
	}
	return;
}

/*Signal interrupts handling*/
void sigint_handler(int sig)
{
	/*shutdown listening socket*/
	printf("Server socket shuting down");
	shutdown(srv_fd,SHUT_RDWR);
	printf("Server socket shutdown");
	_exit(1);
}

/*
server init function, 
server socket is created, then address is assigned to the socket
and listening is done
*/
int init_server()
{
    int ret=0;
	char str_tmp[1024];

    socklen_t len=sizeof(srv_addr);
	struct sigaction sa;
	
	memset(&sa,0,sizeof(struct sigaction));         //memory set
	sa.sa_handler=sigint_handler;                   
	sigaction(SIGINT,&sa,NULL);
	memset(&sa, 0, sizeof(struct sigaction));
	sa.sa_handler = sigchld_handler;
	sigaction(SIGCHLD, &sa, NULL);

	/* create server socket*/
	srv_fd=socket(AF_INET,SOCK_STREAM,0);
	if(srv_fd<0)
	{
		slog("socket() has failed..");
		perror("socket() has failed..\n");
		return(-1);
	}
	slog("server socket created");

	/*assign address to server socket*/
	srv_addr.sin_family = AF_INET;
	srv_addr.sin_port = htons(SERVER_PORT);
	inet_aton(SERVER_IP,&srv_addr.sin_addr);
	
	ret = bind(srv_fd,(struct sockaddr*)&srv_addr,len);
    if(ret!=0)
    {
		slog("bind() failed.");
        perror("bind() failed.\n");
        return(-1);
    }
    
	sprintf(str_tmp,"address given to server : IP = %s PORT = %d",SERVER_IP,SERVER_PORT);
	slog(str_tmp);

	/* listen to server socket*/
	ret = listen(srv_fd,5);
    if(ret!=0)
    {
		slog("listen() failed");
        perror("listen() failed.\n");
        return(-1);
    }
	slog("Listening to server socket on given ip and port");
    return 0;
}

/*client thread creation*/
void * client_thread(void * tmp)
{
	//printf("inside cli thread\n");
	int cli_fd=(int)tmp;
	MYSQL * con = NULL;
	char msg[1024],str[1024];
	con = connect_client(user_name,password,"mysql");
	slog("mysql init successful");
	while(1)
	{		
		/* read data from client & display*/
		read(cli_fd,msg,sizeof(msg));
		printf("Client : %s\n",msg);						
		
		strcpy(str,msg);
		strupr(msg);					
		printf("Server : %s\n",msg);

		/*insert values in table customers*/
		//strcpy(tmp,msg);
		//strcpy(tmp + 1,str);
		char *tmp[] = {msg,str};
		printf("%s %s\n",tmp[0],tmp[1]);
		printf("testing\n");
		add_values(con,"rajput",tmp,2);
		slog("values inserted in table");

		/* write data to client*/
		//fgets(msg,sizeof(msg),stdin);
		write(cli_fd,msg,strlen(msg)+1);
		if(strcmp("EXIT",msg) == 0)
		{
			slog("client_thread exit");
			close(cli_fd);
			mysql_close(con);
			return NULL;
		}
	}
	mysql_close(con);			
	/* close socket*/	
	return NULL;
}

/*close the server fd,log fd and mysql connection*/
void close_all(void)
{
	MYSQL * con=NULL;
	close(srv_fd);
	mysql_close(con);
	close(log_fd);
	return;
}
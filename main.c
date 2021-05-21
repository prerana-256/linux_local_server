#include <stdio.h>
#include <mysql/mysql.h>
#include "server.h"
#include "DBHandler.h"
#include "log.h"
#include "user_menu.h"

/* Driver program */
int main()
{
	pthread_t server_thread, u_menu;   

	char str[2048], client_ip[1024];
	int cli_fd , ret, client_port;
	
	ret=log_init();					
	if(ret != 0)
	{
		slog("LOG INIT FAILED");
		perror("LOG init failed..\n");
		_exit(-1);
	}
	//printf("LOG init successfull....!!!!\n");
	slog("LOG INIT SUCCESSFUL!!");
	
	printf("Who the hell are you????\n");
    printf("User Name : ");
    scanf("%s",user_name);
    printf("Password : ");
    scanf("%s",password);
	
	/*authorize the client*/
	if(connect_client(user_name, password, NULL) == ERR)
	{
		printf("User name or password is invalid\n");
		return ERR;
	}

	ret = init_server();
	if(ret != 0)
	{
		slog("SERVER INIT FAILED");
		perror("Server init failed..\n");
		_exit(-1);
	}
	slog("server init successfull....!!!!");
	pthread_create(&u_menu,NULL,user_menu,NULL);
	
	while(1)
	{
		slog("inside while loop");
		/* accept client connection */
		struct sockaddr_in cli_addr;
		socklen_t len=sizeof(cli_addr);
		slog("Waiting for client connection...");
		cli_fd=accept(srv_fd,(struct sockaddr*)&cli_addr,&len);
		if(cli_fd<0)
		{
				slog("accept() failed..");
				perror("accept() failed..\n");
				if(errno ==EINTR)
						continue;
		}
		client_port=htons(cli_addr.sin_port); 
		inet_ntop(AF_INET,&cli_addr,client_ip,sizeof(client_ip));
		sprintf(str,"address of client : IP = %s PORT = %d",client_ip,client_port);
		slog(str);
		slog("Client connection accepted..");
		
		pthread_create(&server_thread,NULL,client_thread,(void*)cli_fd);
	}
	return 0;	
}

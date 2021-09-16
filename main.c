#include <stdio.h>
#include <mysql/mysql.h>
#include <json-c/json.h>
#include "server.h"
#include "DBHandler.h"
#include "log.h"
#include "user_menu.h"

/* Driver program */
int main()
{
	pthread_t server_thread, u_menu;   
	char str[2048], client_ip[1024];
	int cli_fd, ret, client_port;

	ret = log_init();					
	if(ret != OK)
	{
		perror("LOG init failed..\n");
		_exit(ERR);
	}

	slog("LOG INIT SUCCESSFUL!!");

	printf("User Name: ");
	scanf("%s",username);
	password = getpass("Password: ");

	/*authorize the user*/
	int usr = strcmp(username,"root");
	int pwd = strcmp(password,"pwd@root");
	
	if((usr == 0 && pwd == 0)!=1)
	{
		printf("User name or password is invalid\n");
		slog("User name or password is invalid\n");
		return ERR; 
	}

	ret = init_server();
	if(ret != OK)
	{
		slog("SERVER INIT FAILED");
		perror("Server init failed..\n");
		_exit(ERR);
	}
	slog("server init successfull....!!!!");
	pthread_create(&u_menu, NULL, user_menu, NULL);
	
	while(1)
	{
		slog("Server in listening mode");

		/* accept client connection */

		struct sockaddr_in cli_addr;
		socklen_t len = sizeof(cli_addr);

		slog("Waiting for client connection...");

		cli_fd = accept(srv_fd, (struct sockaddr*)&cli_addr, &len);
		if(cli_fd < 0)
		{
			slog("accept() failed..");
			perror("accept() failed..\n");
			if(errno == EINTR)
				continue;
		}

		client_port = htons(cli_addr.sin_port); 
		inet_ntop(AF_INET, &cli_addr, client_ip, sizeof(client_ip));
		sprintf(str, "address of client : IP = %s PORT = %d", client_ip, client_port);
		slog(str);
		slog("Client connection accepted..");
		
		pthread_create(&server_thread, NULL, client_thread, (void*)(intptr_t)cli_fd);
	}
	return 0;	
}

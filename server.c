		/*server init, interrupt handling, client thread creation, close connections*/

#include "server.h"

/*child interrupts handling*/
void sigchld_handler(int sig)
{
	int s;
	waitpid(-1, &s, 0);
}

/*Signal interrupts handling*/
void sigint_handler(int sig)
{
	/*shutdown listening socket*/
	slog("Server socket shuting down");
	shutdown(srv_fd,SHUT_RDWR);
	slog("Server socket shutdown");
	_exit(sig);
}

/*
server init function, 
server socket is created, then address is assigned to the socket
and listening is done
*/
int init_server()
{
    int ret = 0;
	char str_tmp[1024];

    socklen_t len = sizeof(srv_addr);
	struct sigaction sa;
	
	memset(&sa, 0, sizeof(struct sigaction));         //memory set
	sa.sa_handler = sigint_handler;                   
	sigaction(SIGINT, &sa, NULL);
	memset(&sa, 0, sizeof(struct sigaction));
	sa.sa_handler = sigchld_handler;
	sigaction(SIGCHLD, &sa, NULL);

	/* create server socket*/
	srv_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(srv_fd < 0)
	{
		slog("socket() has failed..");
		perror("socket() has failed..\n");
		return(ERR);
	}
	slog("server socket created");

	/*assign address to server socket*/
	srv_addr.sin_family = AF_INET;
	srv_addr.sin_port = htons(SERVER_PORT);
	inet_aton(SERVER_IP, &srv_addr.sin_addr);
	
	ret = bind(srv_fd, (struct sockaddr*)&srv_addr, len);
    if(ret != OK)
    {
		slog("bind() failed.");
        perror("bind() failed.\n");
        return(ERR);
    }
    
	sprintf(str_tmp, "address given to server : IP = %s PORT = %d", SERVER_IP, SERVER_PORT);
	slog(str_tmp);

	/* listen to server socket*/
	ret = listen(srv_fd, 5);
    if(ret != OK)
    {
		slog("listen() failed");
        perror("listen() failed.\n");
        return(ERR);
    }
	slog("Listening to server socket on given ip and port");
    return OK;
}

/*client thread creation*/
void* client_thread(void *tmp)
{
	const char *db_name, *user_name, *pass_word, *table, *column;
	char msg[1024];
	int ret = 0;
	int cli_fd = (intptr_t)tmp;
	MYSQL * con = NULL;

	char *buf = (char *)malloc(sizeof(char) * 1024);
	if(buf == NULL)
	{
		slog("buf malloc failed");
		return NULL;
	}

    ret = recv(cli_fd, buf, 1024, 0);
    if (ret == ERR)
    {
        slog("database authentication failed");
        return NULL;
    }

    //The string is converted to json
	
    struct json_object *obj = json_tokener_parse(buf);
    struct json_object *json;

    json_object_object_get_ex(obj, "username", &json);
    user_name = json_object_get_string(json);
    
	json_object_object_get_ex(obj, "password", &json);
    pass_word = json_object_get_string(json);

	json_object_object_get_ex(obj, "database", &json);
    db_name = json_object_get_string(json);

	json_object_object_get_ex(obj, "table", &json);
    table = json_object_get_string(json);

	json_object_object_get_ex(obj, "column", &json);
    column = json_object_get_string(json);

	con = connect_client(user_name, pass_word, db_name);
	
	slog("mysql authenticaton and initialization successful");

	free(buf);
	buf = NULL;

	while(1)
	{		
		/* read data from client & display*/
		read(cli_fd, msg, sizeof(msg));

		obj = json_tokener_parse(msg);
    	json_object_object_get_ex(obj, column, &json);
    	const char *data = json_object_get_string(json);

		if(add_values(con, table, data) == ERR)
		{
			slog("Values not added to table");
			write(cli_fd, "FAIL", strlen("FAIL")+1);
			return NULL;
		}

		write(cli_fd, "OK", strlen("OK")+1);

		if(strcmp("EXIT",data) == 0)
		{
			slog("client_thread exit");
			close(cli_fd);
			mysql_close(con);
			return NULL;
		}
	}
	mysql_close(con);			
	return NULL;
}

/*close the server fd,log fd and mysql connection*/
void close_all(void)
{
	MYSQL * con = NULL;
	close(srv_fd);
	mysql_close(con);
	close(log_fd);
	return;
}
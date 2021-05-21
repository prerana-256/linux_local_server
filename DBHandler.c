                    /* Database Handling Functionalities */

#include "DBHandler.h"
#include "server.h"

/* 
   print mysql errors. 
   args - Mysql connection structure
   return value- -1 if error executed  
*/
int finish_with_error(MYSQL * con)
{
    slog("MYSQL ERROR");
    fprintf(stderr,"%s\n",mysql_error(con));
    //mysql_close(con);
    return ERR;
}

/*
    connect with the mysql database
    args - username,password,database as strings
    return value- Mysql connection structure
*/
MYSQL * connect_client(const char * user_name,const char * password,char *db_name)
{
    MYSQL * con = mysql_init(NULL);
    if(con == NULL)
    {
        finish_with_error(con);
    }
    slog("mysql init done");

    if(mysql_real_connect(con, "localhost", user_name, password, db_name, 0, NULL, 0) == NULL)
	{
		finish_with_error(con);
	}
    slog("connect done");
    return con;
}

/*
    create database
    args - Mysql connection struct, database string
    return value - 0 if successfull in database creation
*/
int create_db(MYSQL * con,char *DB_name)
{
    char query[1024];
    sprintf(query,"CREATE DATABASE %s",DB_name);
    if(mysql_query(con, query)) 
    {
        return(finish_with_error(con));
    }
    slog("NEW database create done");
    return OK;
}

/*
    delete database
    args - Mysql connection struct, database string
    return value - 0 if successfull in database deletion
*/
int delete_db(MYSQL * con,char *database_name)
{
    char query[1024];
    sprintf(query,"DROP DATABASE IF EXISTS %s",database_name);
    if(mysql_query(con, query))
    {
        return(finish_with_error(con));
    }
    slog("database deleted\n");
    return OK;
}

/*
    create table 
    args - Mysql connection struct, table string, column string pointer,no of columns
    return value- 
*/
int create_table(MYSQL * con,char *table,char **col,int count)
{
    char query[1024];
    int i=0;
    sprintf(query,"CREATE TABLE %s (%s VARCHAR(255)",table,col[0]);
    for(i = 1; i < count; i++ )	
    {	
        strncat(query,",",1);	
	    strncat(query,col[i],strlen(col[i]));
        strncat(query," VARCHAR(255)",13);
    }
    strncat(query,")",1);
    printf("%s\n",query);
    if(mysql_query(con, query))
    {
        return(finish_with_error(con));
    }
    slog("New table created");
    return;
}

/*
    delete table 
    args - Mysql connection struct, table string, 
    return value- 
*/
int delete_table(MYSQL * con,char * tab_name)
{
    char query[1024];
    sprintf(query,"DROP TABLE IF EXISTS %s",tab_name);
    if (mysql_query(con,query)) 
    {
		return(finish_with_error(con));
	}
    slog("database not exist\n");
    return;
}

/*
    add values into the table
    args- Mysql connection struct, table string, message string pointer,no of values
    return value - 
*/
void add_values(MYSQL *con, char *table, char **msg, int count)
{
    char query[2048];
    int i = 0;
    printf("in add values\n");
    sprintf(query,"INSERT INTO %s VALUES(NULL,\'%s\'",table,msg[0]);
	for(i = 1; i < count; i++ )	
    {	
        strncat(query,",\'",2);	
	    strncat(query,msg[i],strlen(msg[i]));
        strncat(query,"\'",2);
    }
    strncat(query,")",1);

	printf("%s\n",query);
	if (mysql_query(con, query))
	{
		finish_with_error(con);
	}
    slog("query sent");
    return;
}

/*
    create databbase user in mysql
    args- Mysql conn struct, username string, password string
    return value - 
*/
int create_database_user(MYSQL * con,char * usr_name,char* pwd)
{
    char query[1024];
    sprintf(query,"create user '%s'@'localhost'IDENTIFIED BY '%s';",usr_name,pwd);

    if(mysql_query(con,query))
    {
        return(finish_with_error(con));
    }
    slog("new user created");
    sprintf(query,"grant SELECT ON *.* TO '%s'@'localhost';",usr_name);
    if(mysql_query(con,query))
    {
        return(finish_with_error(con));
    }
    slog("permission granted to new user");
    return;
}

/*
    login with new user.
    args- Mysql con struct, username string, password string
    return value - 
*/
int login_with_new_user(MYSQL *con,char *username,char *pwd)
{
    char database[512];
    printf("Database: ");
    scanf("%s",database);
    
    MYSQL * conn= connect_client(username,pwd,database);
    if(conn==NULL)
    {
        return(finish_with_error(con));
    }
    slog("New user login successful");
    return;
}

/*
int is_exist(MYSQL * con,char * name,int q)
{
    if(q==1)
        {
            if(mysql_query(con,"SHOW tables;"))
                finish_with_error(con);
        }

    if(mysql_query(con,"SHOW databases;"))
        finish_with_error(con);
    
    MYSQL_RES *result = mysql_store_result(con);
    
    if(result==NULL)
	{
    	finish_with_error(con);
	}

    MYSQL_ROW row;
    
    while((row=mysql_fetch_row(result)))
    {
        if(strcmp(name,row[0])==0)
        {
            return 0;
        }
    }
    return 1;
}
*/
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
    return ERR;
}

/*
    connect with the mysql database
    args - username,password,database as strings
    return value- Mysql connection structure
*/
MYSQL * connect_client(const char * user_name, const char * password, const char *db_name)
{
    MYSQL * con = mysql_init(NULL);
    if(con == NULL)
        finish_with_error(con);

    slog("mysql init done");

    if(mysql_real_connect(con, "localhost", user_name, password, db_name, 0, NULL, 0) == NULL)
		finish_with_error(con);

    slog("connection to mysql done");
    
    return con;
}
/*
    show status of mysql
    args- Mysql connection struct
    return value - 0 if successfull in showing status
*/
int show_status(MYSQL * con)
{
    printf("\nCLIENT VERSION- %s ",mysql_get_client_info());
    printf("\nHOST info : %s ",mysql_get_host_info(con));
    printf("\nSERVER version as integer : %ld ",mysql_get_server_version(con));
    printf("\nPROTOCOL version : %d ",mysql_get_proto_info(con));
    printf("\nSERVER version : %s \n",mysql_get_server_info(con));
    
    show_curr_user(con);

    slog("MYSQL STATUS DISPLAYED");

    return OK;
}

/* shows current user of Mysql database*/
int show_curr_user(MYSQL * con)
{
    char query2[1024];
    sprintf(query2,"SELECT CURRENT_USER()");
    if(mysql_query(con, query2))
        return(finish_with_error(con));
    
    MYSQL_ROW record;
    MYSQL_RES* rs = mysql_store_result(con);

    while(record = mysql_fetch_row(rs)) 
        printf("CURRENT USER - %s\n", record[0]);

    return OK;
}

/*
    show databases
    args- Mysql connection struct
    return value - 0 if successfull in showing databases
*/
int show_db(MYSQL * con)
{
    char query[1024];

    sprintf(query,"SHOW DATABASES");
    if(mysql_query(con, query))
        return(finish_with_error(con));
    
    MYSQL_ROW record;
    MYSQL_RES* rs = mysql_store_result(con);

    if (rs == NULL) 
    {
        slog("Unable to compile SQL statement to show db\n");
        mysql_close(con);
        return ERR;
    }

    while(record = mysql_fetch_row(rs)) 
        printf("%s\n", record[0]);

    slog("Show database query done");

    return OK;
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
        return(finish_with_error(con));
    
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
        return(finish_with_error(con));
    
    slog("database deleted\n");

    return OK;
}

/*
    show tables from database
    args - Mysql connection struct, database string
    return value - 0 if successfully shown all tables
*/
int show_tables(MYSQL * con, char *DB_name)
{
    char query[1024];

    sprintf(query,"SHOW TABLES");
    if(mysql_query(con, query))
        return(finish_with_error(con));
    
    MYSQL_ROW record;
    MYSQL_RES* rs = mysql_store_result(con);
    if(rs == NULL) 
    {
        slog("Unable to compile SQL statement to show tables\n");
        mysql_close(con);
        return ERR;
    }

    while(record = mysql_fetch_row(rs)) 
        printf("%s\n", record[0]);

    slog("Show database query done");

    return OK;
}
/*
    describe given table
    args - Mysql connection struct, table string
    return value- 0 if successfull.
*/
int describe_t(MYSQL * con, char *table)
{
    char query[1024];
    int i, num_fields;

    sprintf(query,"SELECT * FROM %s",table);
    if(mysql_query(con, query))
        return(finish_with_error(con));
    
    MYSQL_ROW row;
	MYSQL_FIELD *column;
    MYSQL_RES *result = mysql_store_result(con);
    if(result == NULL)
        return(finish_with_error(con));

    num_fields = mysql_num_fields(result);

    printf("COLUMN HEADS -> \n");

    while(column = mysql_fetch_field(result))
        printf(" %s | ",column->name);

    printf("\n--------------------------------------------------------------------------------------------------------------------------------------------\n");	
    printf("ROWS DATA -> \n");

    while(row = mysql_fetch_row(result))
    {
        for(i = 0; i < num_fields; i++)
		    printf(" %s | ", row[i] ? row[i] : "NULL");
        printf("\n");
    }

    mysql_free_result(result);
    result = NULL;

    slog("Describe table query success");

    return OK;
}

/*
    create table
    args - Mysql connection struct, table string, column string pointer,no of columns
    return value- 0 if successfull.
*/
int create_table(MYSQL * con,char *table,char **col,int count)
{
    char query[1024];
    int i=0;

    sprintf(query,"CREATE TABLE %s (%s VARCHAR(255)",table,col[0]);
    for(i = 1; i < count; i++ )	
    {	
        strcat(query, ",");	
	    strcat(query, col[i]);
        strcat(query, " VARCHAR(255)");
    }
    strcat(query, ")");
    
    if(mysql_query(con, query))
        return(finish_with_error(con));
    
    slog("New table created");
    
    return OK;
}
/*
    delete table 
    args - Mysql connection struct, table string, 
    return value- 0 on success
*/
int delete_table(MYSQL * con,char * tab_name)
{
    char query[1024];

    sprintf(query, "DROP TABLE IF EXISTS %s", tab_name);
    if(mysql_query(con,query)) 
		return(finish_with_error(con));
	
    slog("database not exist\n");

    return OK;
}

/*
    add values into the table
    args- Mysql connection struct, table string, message string pointer,no of values
    return value - 0 on success.
*/
int add_values(MYSQL *con, const char *table, const char *data)
{
    char query[2048];
    int i = 0;

    sprintf(query, "INSERT INTO %s VALUES(\'%s\')", table, data);
    /*
    for(i = 1; i < count; i++ )	
    {	
        strncat(query,",\'",2);	
	    strncat(query,msg[i],strlen(msg[i]));
        strncat(query,"\'",2);
    }
    strncat(query,")",1);
    */
	if(mysql_query(con, query))
		return(finish_with_error(con));
	
    slog("VALUES ADDED SUCCESSFULLY");

    return OK;
}

/*
    create databbase user in mysql
    args- Mysql conn struct, username string, password string
    return value - 0 on sucess.
*/
int create_database_user(MYSQL * con,char * usr_name,char * pwd)
{
    char query[1024];
    sprintf(query,"create user '%s'@'localhost'IDENTIFIED BY '%s';",usr_name,pwd);

    if(mysql_query(con,query))
        return(finish_with_error(con));
    
    slog("NEW DATABASE USER CREATED");

    sprintf(query, "grant SELECT ON *.* TO '%s'@'localhost';", usr_name);
    if(mysql_query(con,query))
        return(finish_with_error(con));

    slog("SELECT PRIVILAGE GRANTED");

    sprintf(query, "grant UPDATE ON *.* TO '%s'@'localhost';", usr_name);
    if(mysql_query(con,query))
        return(finish_with_error(con));

    slog("UPDATE PRIVILAGE GRANTED");

    return OK;
}

/* shows all users of mysql database */
int showusers(MYSQL * con)
{
    char query[1024];

    sprintf(query,"select user, max_connections, max_user_connections, password_expired FROM mysql.user");
    if(mysql_query(con, query) == ERR)
        return(finish_with_error(con));
    
    MYSQL_ROW record;
    MYSQL_FIELD *column;
    MYSQL_RES* rs = mysql_store_result(con);
    
    while(column = mysql_fetch_field(rs)) 
        printf(" %15s | ", column->name);

    printf("\n");
    
    while(record = mysql_fetch_row(rs)) 
    {
        printf(" %15s  %15s  %15s  %15s ", record[0], record[1], record[2], record[3]);
        printf("\n\n");
    }

    slog("USERS LISTED SUCCESSFULLY");

    return OK;
}

/* delete given database user */
int drop_user(MYSQL * con, char *user)
{
    char query[1024];

    sprintf(query, "DROP user %s@localhost", user);
    if(mysql_query(con, query) == ERR)
        return(finish_with_error(con));
    
    slog("USER DELETED SUCCESSFULLY");
    return OK;
}

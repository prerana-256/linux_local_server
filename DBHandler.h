/*header files will be included here*/
#ifndef __DBHandler__
#define __DBHandler__
#include<mysql/mysql.h>
#include<stdio.h>
#include "log.h"


#define ERR -1
#define OK 0

int finish_with_error(MYSQL *);
MYSQL * connect_client(const char * , const char *,char *);
void add_values(MYSQL *, char * , char ** ,int);
int is_exist(MYSQL *,char *,int);
int create_db(MYSQL *,char *);
int delete_db(MYSQL *,char *);
int create_table(MYSQL *,char *,char **,int );
int delete_table(MYSQL * ,char *);
int create_database_user(MYSQL *,char *,char*);
int login_with_new_user(MYSQL *,char *,char *);
#endif
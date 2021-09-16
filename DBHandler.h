#ifndef __DBHandler__
#define __DBHandler__

#include <mysql/mysql.h>
#include <stdio.h>
#include "log.h"

int finish_with_error(MYSQL *);
MYSQL * connect_client(const char *, const char *, const char *);
int add_values(MYSQL *, const char *, const char *);
int create_db(MYSQL *, char *);
int delete_db(MYSQL *, char *);
int create_table(MYSQL *, char *, char **, int );
int delete_table(MYSQL *, char *);
int create_database_user(MYSQL *, char *, char*);
int show_db(MYSQL *);
int show_tables(MYSQL *, char *);
int describe_t(MYSQL *, char *);
int show_status(MYSQL *);
int showusers(MYSQL *);
int drop_user(MYSQL *, char *);
int show_curr_user(MYSQL *);

#endif
#ifndef __USER_MENU__
#define __USER_MENU__

#include <stdio.h>
#include <mysql/mysql.h>
#include "DBHandler.h"
#include "server.h"

char username[32];
char *password;

void * user_menu (void *);
int print_menu(void);
void menu_switch(int);
void create_database(void);
void delete_database(void);
void create_db_table(void);
void delete_db_table(void);
void create_db_user(void);
void show_databases(void);
void showtables(void);
void describe_table(void);
void status_show(void);
void show_all_users(void);
void delete_user(void);

#endif
#ifndef __USER_MENU__
#define __USER_MENU__
#include <stdio.h>
#include <mysql/mysql.h>
#include "DBHandler.h"
#include "server.h"


void * user_menu (void *);
int print_menu(void);
void menu_switch(int choice);
void create_database(void);
void delete_database(void);
void create_db_table(void);
void delete_db_table(void);
void create_db_user(void);


#endif
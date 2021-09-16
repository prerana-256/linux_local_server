#include "user_menu.h"

void * user_menu(void * tmp)
{
    while(1)
        menu_switch(print_menu());
}

int print_menu()
{
    char ch;
    int choice;

    printf("Press Enter to see the menu : ");
    scanf("%c",&ch);
    getchar();

    printf("SELECT FROM BELOW LIST\n");
    printf("1.CREATE NEW USER\n");
    printf("2.SHOW ALL USERS\n");
    printf("3.DELETE USER\n");
    printf("4.CREATE DATABASE\n");
    printf("5.SHOW DATABASES\n");
    printf("6.DELETE DATABASE\n");
    printf("7.CREATE TABLE\n");
    printf("8.SHOW TABLES\n");
    printf("9.DESCRIBE TABLE\n");
    printf("10.DELETE TABLE\n");
    printf("11.SHOW MYSQL STATUS\n");
    printf("0.EXIT\n");

    printf("CHOICE : ");
    scanf("%d", &choice);

    return choice;
}

void menu_switch(int choice)
{
    switch(choice)
    {
        case 1:
            create_db_user();
            break;
        case 2:
            show_all_users();
            break;
        case 3:
            delete_user();
            break;
        case 4:
            create_database();
            break;
        case 5:
            show_databases();
            break;
        case 6:
            delete_database();
            break;
        case 7:
            create_db_table();
            break;
        case 8:
            showtables();
            break;
        case 9:
            describe_table();
            break;
        case 10:
            delete_db_table();
            break;
        case 11:
            status_show();
            break;
        case 0:
            {   
                close_all();
                exit(0);
                break;
            }
        default:
            {
                printf("INVALID CHOICE\n");
                close_all();
                exit(-1);
                break;
            }
    }
}

void show_databases(void)
{
    MYSQL *con = connect_client(username, password, NULL);
    if(show_db(con) == ERR)
    {
        mysql_close(con);
        return;
    }

    mysql_close(con);
    
    printf("\nDataBases listed Successfully\n\n");

    return;
}

void create_database(void)
{
    char database_name[512];

    printf("ENTER THE FOLLOWING Details(space NA)\n");
    printf("DataBase Name : ");
    scanf("%s", database_name);
    
    MYSQL *con = connect_client(username, password, NULL);
    if(create_db(con, database_name) == ERR)
    {
        mysql_close(con);
        return;
    }

    mysql_close(con);

    printf("\nDataBase Created Successfully\n\n");
    
    return;
}

void delete_database(void)
{
    char database_name[512];

    printf("ENTER THE FOLLOWING Details(space NA)\n");
    printf("DataBase Name : ");
    scanf("%s", database_name);
    
    MYSQL *con = connect_client(username, password, database_name);
    if(delete_db(con, database_name) == ERR)
    {
        mysql_close(con);
        return;
    }

    mysql_close(con);

    printf("\nDataBase deleted Successfully\n\n");
    
    return;
}

void showtables(void)
{
    char database_name[512];
    
    printf("ENTER THE FOLLOWING Details(space NA)\n");
    printf("DataBase Name : ");
    scanf("%s", database_name);
    
    MYSQL *con = connect_client(username, password, database_name);
    if(show_tables(con, database_name)== ERR)
    {
        mysql_close(con);
        return;
    }

    mysql_close(con);
    
    printf("\nTables listed Successfully\n\n");
    
    return;
}

void describe_table(void)
{
    char database_name[512], table_name[512];
    
    printf("ENTER THE FOLLOWING Details(space NA)\n");
    printf("Database Name : ");
    scanf("%s", database_name);
    printf("Table Name : ");
    scanf("%s", table_name);
    
    MYSQL *con = connect_client(username, password, database_name);
    if(describe_t(con, table_name) == ERR)
    {
        mysql_close(con);
        return;
    }

    mysql_close(con);

    printf("\nTable described Successfully\n\n");
    
    return;
}

void create_db_table(void)
{
    char table_name[512], database[512];
    char ** column_head = NULL;
    int col_count, i;

    printf("ENTER THE FOLLOWING Details(space NA)\n");
    printf("Database : ");
    scanf("%s", database);
    printf("Table Name : ");
    scanf("%s", table_name);
    printf("Enter total number of columns: ");
    scanf("%d", &col_count);

    column_head = (char **) malloc(((col_count-1) * sizeof(char **)));

    for(i=0;i<col_count;i++)
    {
        column_head[i]=(char *) malloc((i * sizeof(char *)));
        printf("Enter column %d name : ",i+1);
        scanf("%s", column_head[i]);
    }

    MYSQL *con = connect_client(username, password, database);
    if(create_table(con, table_name, column_head, col_count) == ERR)
        return;

    free (column_head);
    column_head = NULL;

    mysql_close(con);

    printf("\nTable created Successfully\n\n");

    return;
}

void delete_db_table(void)
{
    char table_name[512], database[512];

    printf("ENTER THE FOLLOWING Details(space NA)\n");
    printf("Database : ");
    scanf("%s", database);
    printf("Table Name : ");
    scanf("%s", table_name);
   
    MYSQL *con = connect_client(username, password, database);
    if(delete_table(con, table_name) == ERR)
        return;

    mysql_close(con);

    printf("\nTable deleted Successfully\n\n");

    return;
}

void create_db_user(void)
{
    char ch, user[512], query[1024];
    
    
    printf("ENTER THE FOLLOWING Details(space NA)\n");
    printf("username : ");
    scanf("%s",user);
    char *pwd = getpass("Password : ");
    //printf("Password : ");
    //scanf("%s",pwd);
    
    MYSQL *con = connect_client(username, password, NULL);
    if(create_database_user(con, user, pwd) == ERR)
        return;
    
    printf("USER created with SELECT and UPDATE privilage Successfully\n");
    
    return;
}

void status_show(void)
{
    MYSQL *con = connect_client(username, password, NULL);
    if(show_status(con) == ERR)
    {
        mysql_close(con);
        return;
    }
    
    mysql_close(con);

    printf("\nStatus shown\n\n");
    
    return;
}

void show_all_users(void)
{
    MYSQL *con = connect_client(username, password, NULL);
    if(showusers(con) == ERR)
    {
        mysql_close(con);
        return;
    }

    mysql_close(con);
    
    printf("\nALL USERS LISTED\n\n");

    return;
}

void delete_user(void)
{
    char user[1024];

    printf("ENTER THE FOLLOWING Details(space NA)\n");
    printf("Username : ");
    scanf("%s", user);

    MYSQL * con = connect_client(username, password, NULL);
    if(drop_user(con, user) == ERR)
    {
        mysql_close(con);
        return;
    }

    mysql_close(con);

    printf("\nUser deleted successfully\n\n");
    
    return;
}
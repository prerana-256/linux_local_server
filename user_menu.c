#include "user_menu.h"

void * user_menu(void * tmp)
{
    char ch;
    int choice;
    while(1)
        menu_switch(print_menu());
}

int print_menu()
{
    int choice;
    printf("SELECT FROM BELOW LIST\n");
    printf("1.CREATE DATABASE\n");
    printf("2.DELETE DATABASE\n");
    printf("3.CREATE TABLE\n");
    printf("4.DELETE TABLE\n");
    printf("5.CREATE NEW USER\n");
    printf("6.EXIT\n");

    printf("CHOICE : ");
    scanf("%d",&choice);
    return choice;
}

void menu_switch(int choice)
{
    switch(choice)
    {
        case 1:
            create_database();
            break;
        case 2:
            delete_database();
            break;
        case 3:
            create_db_table();
            break;
        case 4:
            delete_db_table();
            break;
        case 5:
            create_db_user();
            break;
        case 6:
            {   
                close_all();
                exit(1);
                break;
            }
        default:
            printf("INVALID CHOICE\n");
            break;
    }
}

void create_database(void)
{
    char database_name[512];
    printf("ENTER THE FOLLOWING Details(space NA)\n");
    printf("DataBase Name : ");
    scanf("%s",database_name);
    MYSQL *con = connect_client(user_name, password, NULL);
    
    if(create_db(con, database_name) == ERR)
    {
        mysql_close(con);
        return;
    }

    mysql_close(con);
    printf("DataBase Created Successfully\n");
    return;
}

void delete_database(void)
{
    char database_name[512];
    printf("ENTER THE FOLLOWING Details(space NA)\n");
    printf("DataBase Name : ");
    scanf("%s", database_name);
    MYSQL *con = connect_client(user_name, password, database_name);
    
    if(delete_db(con, database_name) == ERR)
    {
        mysql_close(con);
        return;
    }

    mysql_close(con);
    printf("DataBase deleted Successfully\n");
    return;
}

void create_db_table(void)
{
    char table_name[512],database[512];
    char ** column_head = NULL;
    int col_count,i;

    printf("ENTER THE FOLLOWING Details(space NA)\n");
    printf("Database : ");
    scanf("%s",database);
    printf("Table Name : ");
    scanf("%s", table_name);
    printf("Enter total number of columns: ");
    scanf("%d",&col_count);

    column_head =(char **) malloc(((col_count-1) * sizeof(char **)));

    for(i=0;i<col_count;i++)
    {
        //printf("column %d : ",i+1);
        column_head[i]=(char *) malloc((i * sizeof(char *)));

        scanf("%s",column_head[i]);
        printf("%s\n",column_head[i]);
    }

    MYSQL *con = connect_client(user_name, password, database);
    
    if(create_table(con, table_name,column_head,col_count) == ERR)
        return;

    free (column_head);
    column_head = NULL;
    mysql_close(con);
    printf("Table created Successfully\n");
    return;
}

void delete_db_table(void)
{
    char table_name[512],database[512];

    printf("ENTER THE FOLLOWING Details(space NA)\n");
    printf("Database : ");
    scanf("%s",database);
    printf("Table Name : ");
    scanf("%s", table_name);
   
    MYSQL *con = connect_client(user_name, password, database);
    
    if(delete_table(con, table_name) == ERR)
        return;

    mysql_close(con);
    printf("Table deleted Successfully\n");
    return;
}

void create_db_user(void)
{
    char username[512],password[512],query[1024],ch;
    printf("ENTER THE FOLLOWING Details(space NA)\n");
    printf("User_name : ");
    scanf("%s",username);
    printf("Password : ");
    scanf("%s", password);
    
    MYSQL *con = connect_client(user_name, password,NULL);

    if(create_database_user(con,username,password) == ERR)
        return;

    mysql_close(con);
    printf("USER created with SELECT privilage Successfully\n");
    printf("Wanna Login with new user? (y/n): ");
    scanf("%s",&ch);
    switch(ch)
    {
        case 'y':
            {
                if(login_with_new_user(con,username,password)==ERR)
                    return;
                printf("LOGIN SUCCESSFUL..!!\n");
                break;
            }
        case 'n':
            return;
            break;
        default :
            printf("INVALID OPTION\n");
            break;
    }

    return;
}

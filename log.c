                        /* Log Init,Message printing,and Debugging */
#include "log.h"

/*
    init the log file.
    return 0 if success
*/
int log_init()
{
    
    log_fd = open(LOG_FILE, O_APPEND | O_RDWR | O_CREAT,0744);
    if(log_fd==-1)
    {
        perror("log file not created:\n");
        return 1;
    }
    //printf("log init successful\n");    
    return 0;
}

/*
    write or append into the log file.
    args- message string
    return value - 
*/
void slog(char *str)
{
    char cur_time[128],tmp_str[1024];
    time_t t;
    struct tm* ptm;
    t = time(NULL);
    ptm = localtime(&t);
    strftime(cur_time, 128, "%d-%b-%Y %H:%M:%S", ptm);
    sprintf(tmp_str,"%s : MSG : %s\n",cur_time,str);
    //printf("%s",tmp_str);
    write(log_fd, tmp_str, strlen(tmp_str));
    return;
}
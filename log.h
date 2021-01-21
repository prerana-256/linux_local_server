/*this includes all the header files*/
#ifndef __log__
#define __log__
#include <stdio.h>
#include <sys/types.h>                                                                                                                                                
#include <sys/stat.h>                                                                                                                                                 
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

#define LOG_FILE "./server.log"
int log_fd;
int log_init();
void slog(char * );


#endif

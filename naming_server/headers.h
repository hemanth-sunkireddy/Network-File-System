#ifndef __HEADERS_H
#define __HEADERS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h> 
#include<errno.h>
#include <dirent.h>
#include <limits.h>
#include <time.h>

#define PORT 5000
#define MAX_LENGTH 1024 


typedef struct accessible_path{
    char path[MAX_LENGTH];
}accessible_path;

typedef struct SS_Info{
    char* ip;
    int NM_port;
    int client_port;
    int storage_server_number;
    accessible_path paths_accessible[MAX_LENGTH];
}SS_Info;



// extern char *ip ;
// extern int port;


#endif

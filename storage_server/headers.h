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

#define  MAX_LENGTH 1024
#define MAX_STORAGE_SERVERS 1024
#define FIRST_STORAGE_SERVER_PORT 5000
#define FIRST_IP_ADDRESS 1
#define CURRENT_STORAGE_SERVERS_COUNT 4
#define MAX_CLIENTS_FOR_SERVER 1024

extern char *ip ;
extern int port;
extern int current_storage_server_count; 

typedef struct SS_Info{
    char ip[MAX_LENGTH];
    int NM_port;
    int client_port;
    int storage_server_number;
    int server_socket, client_socket;
    struct sockaddr_in server_address, client_address;
    socklen_t address_size;
}SS_Info;


typedef struct list_of_all_storage_servers{
    SS_Info individual_storage_server[MAX_STORAGE_SERVERS];
}list_of_all_storage_servers;


// void storage_server_initiate(list_of_all_storage_servers SS_database);

#endif

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


typedef struct Packet
{
    int total_chunks;
    char data[MAX_LENGTH];
} Packet;

typedef struct folderInformation{
    char folder_name[MAX_LENGTH];
    int size; 
    char Permissions[MAX_LENGTH];
    char last_modified_time[MAX_LENGTH];
    char created_time[MAX_LENGTH];
    char parent_folder[MAX_LENGTH];
    float total_size_of_folder_in_KB;
    int total_files;
    int total_folders;
}folderInformation;


typedef struct acknowledgmentMessage{
    int operation_number;
    char status_message[MAX_LENGTH];
    Packet file_or_folder_content[MAX_LENGTH];
    folderInformation folder_information;
}acknowledgmentMessage;

typedef struct fileNameAndOperation{
    int operation_number; 
    char name_of_file_or_folder[MAX_LENGTH];
}fileNameAndOperation;


typedef struct accessible_path{
    int storage_server_number;
    char path[MAX_LENGTH];
}accessible_path;

typedef struct SS_Info{
    char* ip;
    int NM_port;
    int client_port;
    
    // accessible_path paths_accessible[MAX_LENGTH];
}SS_Info;



// extern char *ip ;
// extern int port;


#endif

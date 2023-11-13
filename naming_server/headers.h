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
#define NAMING_SERVER_MAIN_PORT 4000
#define MAX_LENGTH 1024 
#define MAX_STORAGE_SERVERS 1024
#define MAX_PATH_LENGTH 10
#define MAX_FOLDER_OR_FILE_LIMIT 10
#define MAX_FILE_LIMIT 5
#define MAX_FOLDER_LIMIT 5
#define CURRENT_STORAGE_SERVERS 4

static int number_of_storage_servers=CURRENT_STORAGE_SERVERS;

typedef struct accessible_path{
    char path[MAX_PATH_LENGTH];
}accessible_path;

typedef struct SS_Info{
    char ip[20];
    int SS_port;
    int client_port;
    int storage_server_number;
    accessible_path paths_accessible[MAX_FOLDER_OR_FILE_LIMIT];
    int num_of_files;
    int num_of_folders;
    int ss_socket;
    struct sockaddr_in ss_address;
    socklen_t ss_address_size;
}SS_Info;

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

typedef struct Data_of_SS_SentToClient{
    int SS_port;
    char* SS_ip;
    int SS_no;
}Data_of_SS_SentToClient;

typedef struct Data_Sent_To_SS{
    fileNameAndOperation filenameandoperation;
}Data_Sent_To_SS;

acknowledgmentMessage obtain_ss_info(SS_Info ssx[MAX_STORAGE_SERVERS],int client_socket, struct acknowledgmentMessage message_status, fileNameAndOperation operation_and_fileorfolder_name);

extern char *ip ;
extern int port;


#endif

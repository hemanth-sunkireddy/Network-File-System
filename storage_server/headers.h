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
#define MAIN_SToRAGE_SERVER 4999
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


typedef struct fileNameAndOperation{
    int storage_server_number;
    int operation_number; 
    char name_of_file_or_folder[MAX_LENGTH];
    int naming_server_or_client;   // 0 for client, 1 for naming server. 
    int create_or_not_create_new_SS_status;  // 0 referes no need to create new SS, 1 refers to create new SS. 
}fileNameAndOperation;


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

acknowledgmentMessage deleting_the_file(char file_name[2*MAX_LENGTH], acknowledgmentMessage message_status);
acknowledgmentMessage creating_the_file(char file_name[2*MAX_LENGTH], acknowledgmentMessage message_status);
acknowledgmentMessage deleting_the_folder(char folder_name[MAX_LENGTH], acknowledgmentMessage message_status);
acknowledgmentMessage creating_the_folder(char folder_name[MAX_LENGTH], acknowledgmentMessage message_status);



acknowledgmentMessage receive_client_request(int client_socket, struct acknowledgmentMessage message_status, int index_of_storage_server);
void send_server_request(acknowledgmentMessage message_status, int client_socket);


#endif

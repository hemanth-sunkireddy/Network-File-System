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
#define MAX_STORAGE_SERVERS 10
#define FIRST_STORAGE_SERVER_PORT 5000
#define FIRST_IP_ADDRESS 1
#define CURRENT_STORAGE_SERVERS_COUNT 4
#define MAX_CLIENTS_FOR_SERVER 1024
#define MAX_LENGTH_OF_PATH 50


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
    int operation_number; 
    char name_of_file_or_folder[MAX_LENGTH];
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

typedef struct fileInformation{
    long int file_size;
    unsigned int file_inode_number;
    int file_owner;
    int file_group;
}fileInformation;

typedef struct directory_or_file_name{
    char file_or_folder_name[MAX_LENGTH_OF_PATH];
}directory_or_file_name;

typedef struct list_in_given_path{
  int total_number_of_files_or_folders;
  directory_or_file_name names_of_file_or_folder[MAX_LENGTH];
}list_in_given_path;

typedef struct Copy_source_dest{
    int operation_number;
    char source_path[MAX_LENGTH_OF_PATH];
    char destination_path[MAX_LENGTH_OF_PATH];
    int source_or_destination_storage_server;    // 0 for source ss, 1 for destination ss. 
    char file_content[MAX_LENGTH];
    int port_number; 
}Copy_source_dest;

typedef struct Data_of_SS_SentForCopying{
    int SS_port;
    char SS_ip[MAX_LENGTH];
    int SS_no;
}Data_of_SS_SentForCopying;

typedef struct acknowledgmentMessage{
    int operation_number;
    char status_message[MAX_LENGTH];
    Packet file_or_folder_content[MAX_LENGTH];
    folderInformation folder_information;
    fileInformation file_information;
    list_in_given_path list_of_folder;
}acknowledgmentMessage;

acknowledgmentMessage deleting_the_file(char file_name[2*MAX_LENGTH], acknowledgmentMessage message_status);
acknowledgmentMessage creating_the_file(char file_name[2*MAX_LENGTH], acknowledgmentMessage message_status);
acknowledgmentMessage deleting_the_folder(char folder_name[MAX_LENGTH], acknowledgmentMessage message_status);
acknowledgmentMessage creating_the_folder(char folder_name[MAX_LENGTH], acknowledgmentMessage message_status);
acknowledgmentMessage additional_information_of_folder(char folder_name[MAX_LENGTH], acknowledgmentMessage message_status);
acknowledgmentMessage reading_the_file(char file_name[2*MAX_LENGTH], acknowledgmentMessage message_status); 
acknowledgmentMessage additional_information_of_file(char file_name[MAX_LENGTH], acknowledgmentMessage message_status);
acknowledgmentMessage listing_all_files_and_folders(char folder_name[MAX_LENGTH], acknowledgmentMessage message_status);
acknowledgmentMessage writing_the_file(char file_name[2*MAX_LENGTH], acknowledgmentMessage message_status, char data_from_client[MAX_LENGTH]);
acknowledgmentMessage copying_the_file(char final_path_of_the_source[MAX_LENGTH_OF_PATH], Copy_source_dest details_of_source_destination_storage_server, acknowledgmentMessage message_status, int destination_ss_port_no);


acknowledgmentMessage receive_client_request(int client_socket, struct acknowledgmentMessage message_status, int index_of_storage_server, int* current_storage_servers);
void send_server_request(acknowledgmentMessage message_status, int client_socket);
void create_socket(list_of_all_storage_servers *database, int index_of_storage_server);
void bind_socket(list_of_all_storage_servers *database, int index_of_storage_server);
void start_listening(list_of_all_storage_servers *database, int index_of_storage_server);

#endif

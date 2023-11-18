
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

#define MAX_LENGTH 1024
#define MAX_LENGTH_OF_PATH 10



typedef struct NS{
    int port;
    char IP[MAX_LENGTH];
    int SS_no;
}NS_reply;


typedef struct fileNameAndOperation{
    int operation_number; 
    char name_of_file_or_folder[MAX_LENGTH];
}fileNameAndOperation;


typedef struct source_dest{
    char source_path[1024];
    char destination_path[1024];
}source_dest;



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



typedef struct acknowledgmentMessage{
    int operation_number;
    char status_message[MAX_LENGTH];
    Packet file_or_folder_content[MAX_LENGTH];
    folderInformation folder_information;
    fileInformation file_information;
    list_in_given_path list_of_folder;
}acknowledgmentMessage;




void client_receving_data(int client_socket,fileNameAndOperation file_or_folder_details);



//git status
//git add .
//git status
//git commit -m "update"
//git push

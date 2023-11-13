
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

// char *ip;// = "127.0.0.1";
int port=4000; // = PORT;
char message[MAX_LENGTH];

typedef struct NS{
    int port;
    char IP[100];
    int SS_no;
}NS_reply;


typedef struct fileNameAndOperation{
    int operation_number; 
    char name_of_file_or_folder[MAX_LENGTH];
}fileNameAndOperation;



void Info(fileNameAndOperation *file_or_folder_details){


        // fileNameAndOperation file_or_folder_details;

        // Displaying all options in the terminal of the client side.
        printf("Choose one number from below:\n");
        printf("-------------------------------\n");
        printf("1 - Reading a File\n");
        printf("2 - Writing a File\n");
        printf("3 - Deleting a File\n");
        printf("4 - Creating a File\n");
        printf("5 - Getting Additional information of File \n");
        printf("6 - Getting Additional information of Folder\n");
        printf("7 - Listing all Files and Folders\n");
        printf("8 - Deleting a Folder\n");
        printf("9 - Creating a Folder\n");
        printf("-------------------------------\n");

        printf("Enter the number:");
        int option_choosing_number; 
        scanf("%d", &(file_or_folder_details->operation_number));

        if ( file_or_folder_details->operation_number <= 5){ 
                printf("Enter the file name: ");
        }
        else{
                printf("Enter the Folder name: ");
        }
        scanf("%s", file_or_folder_details->name_of_file_or_folder);
}


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







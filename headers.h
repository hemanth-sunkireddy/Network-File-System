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

#define PORT 5000
#define MAX_LENGTH 1024 


typedef struct Packet
{
    int total_chunks;
    char data[MAX_LENGTH];
} Packet;



typedef struct acknowledgmentMessage{
    char status_message[MAX_LENGTH];
    Packet file_or_folder_content[MAX_LENGTH];
}acknowledgmentMessage;

acknowledgmentMessage reading_the_file(char file_name[MAX_LENGTH], acknowledgmentMessage message_status); 
void writing_the_file(char file_name[MAX_LENGTH]);
void deleting_the_file(char file_name[MAX_LENGTH]);
void creating_the_file(char file_name[MAX_LENGTH]);
void additional_information_of_file(char file_name[MAX_LENGTH]);
void additional_information_of_folder(char folder_name[MAX_LENGTH]);
void writing_the_folder(char folder_name[MAX_LENGTH]);
void deleting_the_folder(char folder_name[MAX_LENGTH]);
void creating_the_folder(char folder_name[MAX_LENGTH]);
void listing_all_files_and_folders(char folder_name[MAX_LENGTH]);




acknowledgmentMessage receive_client_request(int client_socket, struct acknowledgmentMessage message_status);
void send_client_request(int sock);
void send_server_request(acknowledgmentMessage message_status, int client_socket);
void receive_server_request(int client_socket);



typedef struct fileNameAndOperation{
    int operation_number; 
    char name_of_file_or_folder[MAX_LENGTH];
}fileNameAndOperation;



extern char *ip ;
extern int port;


#endif

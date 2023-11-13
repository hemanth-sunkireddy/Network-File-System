#include "headers.h"

char *ip = "127.0.0.1";
int port = 8000;

void CreateandDeleteOperation(int client_socket, struct acknowledgmentMessage message_status,char* path,fileNameAndOperation file_or_folder_details)
{
    SS_Info ssx;
    recv(client_socket, &file_or_folder_details, MAX_LENGTH, 0);
    char file_or_folder_name[MAX_LENGTH];
    strcpy(file_or_folder_name,file_or_folder_details.name_of_file_or_folder);
    int operation_number=file_or_folder_details.operation_number; 
    int nm_as_client_socket;
    struct sockaddr_in nm_as_client_address;
    socklen_t nm_as_client_address_size;
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0){
        perror("Socket error");
        exit(1);
    }
    else{
        printf("TCP CLIENT SOCKET CREATED.\n");
    }
    memset(&nm_as_client_address, '\0', sizeof(nm_as_client_address));
    nm_as_client_address.sin_family = AF_INET;
    nm_as_client_address.sin_port = port;
    nm_as_client_address.sin_addr.s_addr = inet_addr(ip);
    connect(client_socket, (struct sockaddr*)&nm_as_client_address, sizeof(nm_as_client_address));
    printf("Connected to the server.\n\n");
    if(operation_number==4 || operation_number==9){
        if(strchr(path, '/')==NULL){                  //single file or directory
            if(!ssx){                                 //condition for storge server being not full
                snprintf(ssx.paths_accessible[ssx.num_of_paths++].path,MAX_LENGTH,"%s/%s",path,file_or_folder_name);
                printf("Creation successful\n");
                strcpy(message_status.status_message,"Creation successful");
                send(nm_as_client_socket,&message_status,MAX_LENGTH,0);
            }
            else{
                //create new storage server
                SS_Info ss_new;
                ss_new_no++;
                snprintf(ss_new.paths_accessible[ss_new.num_of_paths++].path,MAX_LENGTH,"%s/%s",path,file_or_folder_name);
                printf("Creation successful\n");
                strcpy(message_status.status_message,"Creation successful");
                send(nm_as_client_socket,&message_status,MAX_LENGTH,0);
            }
        }
        else{
            //search implementation
            if(path){
                printf("Path found\n");
                snprintf(ssx.paths_accessible[ssx.num_of_paths++].path,MAX_LENGTH,"%s/%s",path,file_or_folder_name);
                printf("Creation successful\n");
                strcpy(message_status.status_message,"Creation successful");
                send(nm_as_client_socket,&message_status,MAX_LENGTH,0);
            }
            else{
                printf("Path not found\n");
                printf("Creation not successful since path not found\n");
                strcpy(message_status.status_message,"Creation successful since path not found");
                send(nm_as_client_socket,&message_status,MAX_LENGTH,0);
            }
        }
    }
    else if(operation_number==3 || operation_number==8){
        //search through all the accessible paths
        if(path){
            int len=strlen(file_or_folder_name);
            snprintf(path,strlen(path)-len,"%s",path);
            ssx.num_of_paths--;
            printf("Path found and deletion successful");
            strcpy(message_status.status_message,"Deletion successful since path not found");
            send(nm_as_client_socket,&m#include "headers.h"

char *ip = "127.0.0.1";
int port = 8000;

void CreateandDeleteOperation(int client_socket, struct acknowledgmentMessage message_status,char* path,fileNameAndOperation file_or_folder_details)
{
    SS_Info ssx;
    recv(client_socket, &file_or_folder_details, MAX_LENGTH, 0);
    char file_or_folder_name[MAX_LENGTH];
    strcpy(file_or_folder_name,file_or_folder_details.name_of_file_or_folder);
    int operation_number=file_or_folder_details.operation_number; 
    int nm_as_client_socket;
    struct sockaddr_in nm_as_client_address;
    socklen_t nm_as_client_address_size;
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0){
        perror("Socket error");
        exit(1);
    }
    else{
        printf("TCP CLIENT SOCKET CREATED.\n");
    }
    memset(&nm_as_client_address, '\0', sizeof(nm_as_client_address));
    nm_as_client_address.sin_family = AF_INET;
    nm_as_client_address.sin_port = port;
    nm_as_client_address.sin_addr.s_addr = inet_addr(ip);
    connect(client_socket, (struct sockaddr*)&nm_as_client_address, sizeof(nm_as_client_address));
    printf("Connected to the server.\n\n");
    if(operation_number==4 || operation_number==9){
        if(strchr(path, '/')==NULL){                  //single file or directory
            if(!ssx){                                 //condition for storge server being not full
                snprintf(ssx.paths_accessible[ssx.num_of_paths++].path,MAX_LENGTH,"%s/%s",path,file_or_folder_name);
                printf("Creation successful\n");
                strcpy(message_status.status_message,"Creation successful");
                send(nm_as_client_socket,&message_status,MAX_LENGTH,0);
            }
            else{
                //create new storage server
                SS_Info ss_new;
                ss_new_no++;
                snprintf(ss_new.paths_accessible[ss_new.num_of_paths++].path,MAX_LENGTH,"%s/%s",path,file_or_folder_name);
                printf("Creation successful\n");
                strcpy(message_status.status_message,"Creation successful");
                send(nm_as_client_socket,&message_status,MAX_LENGTH,0);
            }
        }
        else{
            //search implementation
            if(path){
                printf("Path found\n");
                snprintf(ssx.paths_accessible[ssx.num_of_paths++].path,MAX_LENGTH,"%s/%s",path,file_or_folder_name);
                printf("Creation successful\n");
                strcpy(message_status.status_message,"Creation successful");
                send(nm_as_client_socket,&message_status,MAX_LENGTH,0);
            }
            else{
                printf("Path not found\n");
                printf("Creation not successful since path not found\n");
                strcpy(message_status.status_message,"Creation successful since path not found");
                send(nm_as_client_socket,&message_status,MAX_LENGTH,0);
            }
        }
    }
    else if(operation_number==3 || operation_number==8){
        //search through all the accessible paths
        if(path){
            int len=strlen(file_or_folder_name);
            snprintf(path,strlen(path)-len,"%s",path);
            ssx.num_of_paths--;
            printf("Path found and deletion successful");
            strcpy(message_status.status_message,"Deletion successful since path not found");
            send(nm_as_client_socket,&message_status,MAX_LENGTH,0);
        }
        else{
            printf("Path not found");
            strcpy(message_status.status_message,"Deletion successful since path not found");
            send(nm_as_client_socket,&message_status,MAX_LENGTH,0);
        }
    }
}

acknowledgmentMessage receive_initialized_ss_info(int ss_socket, acknowledgmentMessage message_status) {
    SS_Info ssx;
    ssx.paths_accessible[10];

    recv(ss_socket, &ssx, sizeof(SS_Info), 0);
    strcpy(message_status.status_message, "Storage server info received successfully");
    printf("IP: %s\n", ssx.ip);
    printf("NM_Port: %d\n", ssx.NM_port);
    printf("Client_Port: %d\n", ssx.client_port);
    printf("SS Number: %d\n", ssx.storage_server_number);
    for(int i=0;i<1;i++){
        printf("%s\n",ssx.paths_accessible->path);
    }
    return message_status;
}essage_status,MAX_LENGTH,0);
        }
        else{
            printf("Path not found");
            strcpy(message_status.status_message,"Deletion successful since path not found");
            send(nm_as_client_socket,&message_status,MAX_LENGTH,0);
        }
    }
}

acknowledgmentMessage receive_initialized_ss_info(int ss_socket, acknowledgmentMessage message_status) {
    SS_Info ssx;
    ssx.paths_accessible[10];

    recv(ss_socket, &ssx, sizeof(SS_Info), 0);
    strcpy(message_status.status_message, "Storage server info received successfully");
    printf("IP: %s\n", ssx.ip);
    printf("NM_Port: %d\n", ssx.NM_port);
    printf("Client_Port: %d\n", ssx.client_port);
    printf("SS Number: %d\n", ssx.storage_server_number);
    for(int i=0;i<1;i++){
        printf("%s\n",ssx.paths_accessible->path);
    }
    return message_status;
}
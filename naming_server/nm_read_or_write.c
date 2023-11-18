#include "headers.h"


void ReadorWriteOperation(SS_Info ssx[MAX_STORAGE_SERVERS], acknowledgmentMessage message_status, fileNameAndOperation FilenameAndOperation, int ss_socket)
{
    printf("HELLO");
    printf("Entered\n");
    char path[MAX_PATH_LENGTH];
    strcpy(path, "HELLO");
    printf("HELLO STRING: %s\n", path);
    strcpy(path,FilenameAndOperation.name_of_file_or_folder);
    printf("Path name: %s",path);
    
    for(int i=0;i<number_of_storage_servers;i++){
        for(int j=0;j<ssx[i].num_of_files+ssx[i].num_of_folders;j++){
            if((strncmp(path,ssx[i].paths_accessible[j].path,strlen(ssx[i].paths_accessible[j].path))==0 && path[strlen(ssx[i].paths_accessible[j].path)]=='\0') || (strncmp(path,ssx[i].paths_accessible[j].path,strlen(ssx[i].paths_accessible[j].path))==0 && path[strlen(ssx[i].paths_accessible[j].path)]=='/' && path[strlen(ssx[i].paths_accessible[j].path) + 1] != '\0')) {    
                Data_of_SS_SentToClient data_sent;
                data_sent.SS_port=ssx[0].SS_port;
                printf("ASSIGNED PORT SUCCESS.");
                strcpy(data_sent.SS_ip,ssx[0].ip);
                printf("ASSIGNED IP SUCCESS.");
                data_sent.SS_port=ssx[0].SS_port;
                printf("ASSIGNED PORT SUCCESS.");
                strcpy(data_sent.SS_ip,ssx[0].ip);
                printf("ASSIGNED IP SUCCESS.");
                data_sent.SS_no=ssx[0].storage_server_number;
                printf("STORAGE NUMBER SUCCESS.");
                int port=NAMING_SERVER_MAIN_PORT;
                int client_socket=socket(AF_INET, SOCK_STREAM, 0);
                struct sockaddr_in client_address;
                socklen_t client_address_size;
                printf("HELLO");
                        if (client_socket < 0){
                            perror("Socket error");
                            exit(1);
                        }
                        else{
                            printf("TCP CLIENT SOCKET CREATED for NS.\n");
                        }
                        memset(&client_address, '\0', sizeof(client_address));
                        client_address.sin_family = AF_INET;
                        client_address.sin_port = port;
                        client_address.sin_addr.s_addr = inet_addr(ip);
                        //printf("%d %d %d %d\n",ssx[i].SS_port,ssx[i].storage_server_number,ssx[i].num_of_files,ssx[i].num_of_folders);
                        // Connect to the server
                        connect(client_socket, (struct sockaddr*)&client_address, sizeof(client_address));
                        printf("Connected to the client.\n\n");
                //         char message[1024];
                //         strcpy(message, "HELLO ECHO FROM NAMING SERVER.");
                // send(ss_socket,message,MAX_LENGTH,0);
                send(ss_socket,&data_sent,sizeof(Data_of_SS_SentToClient),0);
                printf("Sent File or folder name and Operation number successfully\n");
            }
        }
    }
}
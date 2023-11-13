#include "headers.h"


// void CreateandDeleteOperation(int client_socket, struct acknowledgmentMessage message_status,fileNameAndOperation FilenameAndOperation)
// {
//     SS_Info ssx;
//     char file_or_folder_name[MAX_LENGTH];
//     strcpy(file_or_folder_name,FilenameAndOperation.name_of_file_or_folder);
//     int operation_number=FilenameAndOperation.operation_number; 
//     int nm_as_client_socket;
//     struct sockaddr_in nm_as_client_address;
//     socklen_t nm_as_client_address_size;
//     client_socket = socket(AF_INET, SOCK_STREAM, 0);
//     if (client_socket < 0){
//         perror("Socket error");
//         exit(1);
//     }
//     else{
//         printf("TCP CLIENT SOCKET CREATED.\n");
//     }
//     memset(&nm_as_client_address, '\0', sizeof(nm_as_client_address));
//     nm_as_client_address.sin_family = AF_INET;
//     nm_as_client_address.sin_port = port;
//     nm_as_client_address.sin_addr.s_addr = inet_addr(ip);
//     connect(client_socket, (struct sockaddr*)&nm_as_client_address, sizeof(nm_as_client_address));
//     printf("Connected to the server.\n\n");
//     if(operation_number==4 || operation_number==9){
//         if(strchr(path, '/')==NULL){                  //single file or directory
//             // if(!ssx){        
//             if(1){                        //condition for storge server being not full
//                 // snprintf(ssx.paths_accessible[ssx.num_of_paths++].path,MAX_LENGTH,"%s/%s",path,file_or_folder_name);
//                 // printf("Creation successful\n");
//                 // strcpy(message_status.status_message,"Creation successful");
//                 // send(nm_as_client_socket,&message_status,MAX_LENGTH,0);
//             }
//             else{
//                 //create new storage server
//                 // SS_Info ss_new;
//                 // ss_new_no++;
//                 // snprintf(ss_new.paths_accessible[ss_new.num_of_paths++].path,MAX_LENGTH,"%s/%s",path,file_or_folder_name);
//                 // printf("Creation successful\n");
//                 // strcpy(message_status.status_message,"Creation successful");
//                 // send(nm_as_client_socket,&message_status,MAX_LENGTH,0);
//             }
//         }
//         // else{
//         //     //search implementation
//         //     if(path){
//         //         printf("Path found\n");

//         //         char forward_slash_character[1];
                
//         //         snprintf(ssx.paths_accessible[ssx.num_of_paths++].path,MAX_LENGTH,"%s/%s",path,file_or_folder_name);
//         //         printf("Creation successful\n");
//         //         strcpy(message_status.status_message,"Creation successful");
//         //         send(nm_as_client_socket,&message_status,MAX_LENGTH,0);
//         //     }
//         //     else{
//         //         printf("Path not found\n");
//         //         printf("Creation not successful since path not found\n");
//         //         strcpy(message_status.status_message,"Creation successful since path not found");
//         //         send(nm_as_client_socket,&message_status,MAX_LENGTH,0);
//         //     }
//         // }
//     }
//     else if(operation_number==3 || operation_number==8){
//         //search through all the accessible paths
//         if(path){
//             int len=strlen(file_or_folder_name);
//             snprintf(path,strlen(path)-len,"%s",path);
//             ssx.num_of_paths--;
//             printf("Path found and deletion successful");
//             strcpy(message_status.status_message,"Deletion successful since path not found");
//             send(nm_as_client_socket,&message_status,MAX_LENGTH,0);
//         }
//         else{
//             printf("Path not found");
//             strcpy(message_status.status_message,"Deletion successful since path not found");
//             send(nm_as_client_socket,&message_status,MAX_LENGTH,0);
//         }
//     }
// }

SS_Info* CreateNewStorageServer(SS_Info ssx[MAX_STORAGE_SERVERS])
{
    ssx[number_of_storage_servers].SS_port=5000+number_of_storage_servers-1;
    ssx[number_of_storage_servers].storage_server_number=number_of_storage_servers-1;
}

void CreateandDeleteOperation(SS_Info ssx[MAX_STORAGE_SERVERS], struct acknowledgmentMessage message_status,fileNameAndOperation FilenameAndOperation)
{
    int operation_num=FilenameAndOperation.operation_number;
    char path[20];
    strcpy(path,FilenameAndOperation.name_of_file_or_folder);
    if(operation_num==4 || operation_num==9){
        if(strchr(path, '/')==NULL){                  //single file or directory
            if(1){                        //condition for storge server being not full
                for(int i=0;i<number_of_storage_servers;i++){
                    if(operation_num==4){
                        if(ssx[i].num_of_files==MAX_FILE_LIMIT){
                            continue;
                        }
                        int port=5000+i;
                        ssx[i].ss_socket = socket(AF_INET, SOCK_STREAM, 0);
                        if (ssx[i].ss_socket < 0){
                            perror("Socket error");
                            exit(1);
                        }
                        else{
                            printf("TCP SS_CLIENT SOCKET CREATED for NS.\n");
                        }

                        memset(&ssx[i].ss_address, '\0', sizeof(ssx[i].ss_address));
                        ssx[i].ss_address.sin_family = AF_INET;
                        ssx[i].ss_address.sin_port = port;
                        ssx[i].ss_address.sin_addr.s_addr = inet_addr(ip);

                        // Connect to the server
                        connect(ssx[i].ss_socket, (struct sockaddr*)&ssx[i].ss_address, sizeof(ssx[i].ss_address));
                        printf("Connected to the server.\n\n");
                        send(ssx[i].ss_socket,&FilenameAndOperation,sizeof(fileNameAndOperation),0);
                        printf("Sent File or folder name and Operation number successfully\n");
                        if(operation_num==4){
                            ssx[i].num_of_files++;
                        }
                        else{
                            ssx[i].num_of_folders++;
                        }
                    }
                    else{
                        if(ssx[i].num_of_folders==MAX_FOLDER_LIMIT){
                            continue;
                        }
                        int port=5000+i;
                        ssx[i].ss_socket = socket(AF_INET, SOCK_STREAM, 0);
                        if (ssx[i].ss_socket < 0){
                            perror("Socket error");
                            exit(1);
                        }
                        else{
                            printf("TCP SS_CLIENT SOCKET CREATED for NS.\n");
                        }

                        memset(&ssx[i].ss_address, '\0', sizeof(ssx[i].ss_address));
                        ssx[i].ss_address.sin_family = AF_INET;
                        ssx[i].ss_address.sin_port = port;
                        ssx[i].ss_address.sin_addr.s_addr = inet_addr(ip);

                        // Connect to the server
                        connect(ssx[i].ss_socket, (struct sockaddr*)&ssx[i].ss_address, sizeof(ssx[i].ss_address));
                        printf("Connected to the server.\n\n");
                        send(ssx[i].ss_socket,&FilenameAndOperation,sizeof(fileNameAndOperation),0);
                        printf("Sent File or folder name and Operation number successfully\n");
                        if(operation_num==4){
                            ssx[i].num_of_files++;
                        }
                        else{
                            ssx[i].num_of_folders++;
                        }
                    }
                }
            }
            else{
                number_of_storage_servers++;
                //create new storage server
                SS_Info* ss=CreateNewStorageServer(ssx);
                int port=5000+number_of_storage_servers-1;
                        ss[number_of_storage_servers-1].ss_socket = socket(AF_INET, SOCK_STREAM, 0);
                        if (ssx[number_of_storage_servers-1].ss_socket < 0){
                            perror("Socket error");
                            exit(1);
                        }
                        else{
                            printf("TCP SS_CLIENT SOCKET CREATED for NS.\n");
                        }

                        memset(&ssx[number_of_storage_servers-1].ss_address, '\0', sizeof(ssx[number_of_storage_servers-1].ss_address));
                        ssx[number_of_storage_servers-1].ss_address.sin_family = AF_INET;
                        ssx[number_of_storage_servers-1].ss_address.sin_port = port;
                        ssx[number_of_storage_servers-1].ss_address.sin_addr.s_addr = inet_addr(ip);

                        // Connect to the server
                        connect(ssx[number_of_storage_servers-1].ss_socket, (struct sockaddr*)&ssx[number_of_storage_servers-1].ss_address, sizeof(ssx[number_of_storage_servers-1].ss_address));
                        printf("Connected to the server.\n\n");
                        send(ssx[number_of_storage_servers-1].ss_socket,&FilenameAndOperation,sizeof(fileNameAndOperation),0);
                        printf("Sent File or folder name and Operation number successfully\n");
                        if(operation_num==4){
                            ssx[number_of_storage_servers-1].num_of_files++;
                        }
                        else{
                            ssx[number_of_storage_servers-1].num_of_folders++;
                        }
            }
        }
        else{
            for(int i=0;i<CURRENT_STORAGE_SERVERS;i++){
                char *slash1 = strchr(path, '/');
                char *slash2 = strchr(ssx[i].paths_accessible->path, '/');
                // Calculate the length up to the first '/'
                size_t len1 = (size_t)(slash1 - path);
                size_t len2 = (size_t)(slash2 - ssx[i].paths_accessible->path);
                char path_dup[len1+1];
                char accessible_path_dup[len2+1];
                strncpy(path_dup,path,len1);
                strncpy(accessible_path_dup,ssx[i].paths_accessible->path,len2);
                // Compare strings up to the first '/'
                int result = strcmp(path_dup, accessible_path_dup);
                if(result==0){
                    printf("File or folder is in storage server %d",i);
                    int port=5000+i;
                    ssx[i].ss_socket = socket(AF_INET, SOCK_STREAM, 0);
                        if (ssx[i].ss_socket < 0){
                            perror("Socket error");
                            exit(1);
                        }
                        else{
                            printf("TCP SS_CLIENT SOCKET CREATED for NS.\n");
                        }

                        memset(&ssx[i].ss_address, '\0', sizeof(ssx[i].ss_address));
                        ssx[i].ss_address.sin_family = AF_INET;
                        ssx[i].ss_address.sin_port = port;
                        ssx[i].ss_address.sin_addr.s_addr = inet_addr(ip);

                        // Connect to the server
                        connect(ssx[i].ss_socket, (struct sockaddr*)&ssx[i].ss_address, sizeof(ssx[i].ss_address));
                        printf("Connected to the server.\n\n");
                    send(ssx[i].ss_socket,&FilenameAndOperation,sizeof(fileNameAndOperation),0);
                    printf("Sent File or folder name and Operation number successfully\n");
                    if(operation_num==4){
                        ssx[i].num_of_files++;
                    }
                    else{
                        ssx[i].num_of_folders++;
                    }
                    break;
                }
                else{
                    printf("Path not found\n");
                    printf("Creation not successful since path not found\n");
                    strcpy(message_status.status_message,"Creation successful since path not found");
                    //send(nm_as_client_socket,&message_status,MAX_LENGTH,0);
                }
            }
        }
    }
    
}

acknowledgmentMessage obtain_ss_info(SS_Info ssx[MAX_STORAGE_SERVERS],int ss_socket, acknowledgmentMessage message_status, fileNameAndOperation operation_and_fileorfolder_name){
    int operation_no=operation_and_fileorfolder_name.operation_number;
    if(operation_no==3 || operation_no==4 || operation_no==8 || operation_no==9){
        CreateandDeleteOperation(ssx,message_status,operation_and_fileorfolder_name);
    }
    else if(operation_no==1 || operation_no==2 || operation_no==5 || operation_no==6 || operation_no==7){
        //ReadorWriteOperation(ssx,ss_socket,message_status,operation_and_fileorfolder_name);
    }
    else{
        printf("Invalid operation\n");
        //message_status ack
    }
    //strcpy(message_status.status_message, "Storage server info sent successfully");
    // printf("IP: %s\n", ssx.ip);
    // printf("NM_Port: %d\n", ssx.SS_port);
    // printf("Client_Port: %d\n", ssx.client_port);
    // printf("SS Number: %d\n", ssx.storage_server_number);
    // for(int i=0;i<1;i++){
    //     printf("%s\n",ssx.paths_accessible->path);
    // }
    return message_status;
}

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

void saveFileCounts(SS_Info ssx[MAX_STORAGE_SERVERS]) {
    FILE *file = fopen("file_counts.txt", "w");
    if (file == NULL) {
        perror("Error opening file");
        exit(1);
    }

    for (int i = 0; i < number_of_storage_servers; i++) {
        fprintf(file, "%d\n", ssx[i].num_of_files);
    }

    fclose(file);
}

void loadFileCounts(SS_Info ssx[MAX_STORAGE_SERVERS]) {
    FILE *file = fopen("file_counts.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(1);
    }

    for (int i = 0; i < number_of_storage_servers; i++) {
        fscanf(file, "%d", &(ssx[i].num_of_files));
    }

    fclose(file);
}

void saveFolderCounts(SS_Info ssx[MAX_STORAGE_SERVERS]) {
    FILE *file = fopen("folder_counts.txt", "w");
    if (file == NULL) {
        perror("Error opening file");
        exit(1);
    }

    for (int i = 0; i < number_of_storage_servers; i++) {
        fprintf(file, "%d\n", ssx[i].num_of_folders);
    }

    fclose(file);
}

void loadFolderCounts(SS_Info ssx[MAX_STORAGE_SERVERS]) {
    FILE *file = fopen("folder_counts.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(1);
    }

    for (int i = 0; i < number_of_storage_servers; i++) {
        fscanf(file, "%d", &(ssx[i].num_of_folders));
    }

    fclose(file);
}

void CreateandDeleteOperation(SS_Info ssx[MAX_STORAGE_SERVERS], struct acknowledgmentMessage message_status, fileNameAndOperation FilenameAndOperation) {
    int operation_num = FilenameAndOperation.operation_number;
    char path[MAX_PATH_LENGTH];
    strcpy(path, FilenameAndOperation.name_of_file_or_folder);
    printf("NUm of storage servers: %d\n",number_of_storage_servers);
    // Load the file counts at the beginning of the program
    loadFileCounts(ssx);
    loadFolderCounts(ssx);
    if(operation_num==4 || operation_num==9){ // Create file or folder
        if (strchr(path, '/') == NULL) { // Check if it's a single file or directory
            int i;
            for (i = 0; i < number_of_storage_servers; i++) {
                if (operation_num == 4 && ssx[i].num_of_files < MAX_FILE_LIMIT) {
                    // Creating a file
                    // Your logic to connect and create a file in the storage server i
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
                        printf("Connected to the storage server %d.\n\n",i);
                        send(ssx[i].ss_socket,&FilenameAndOperation,sizeof(fileNameAndOperation),0);
                        printf("Sent File or folder name and Operation number successfully\n");
                    ssx[i].num_of_files++;
                    saveFileCounts(ssx); // Save the updated file counts
                    break;
                } else if (operation_num == 9 && ssx[i].num_of_folders < MAX_FOLDER_LIMIT) {
                    // Creating a folder
                    // Your logic to connect and create a folder in the storage server i
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
                        printf("Connected to the storage server %d.\n\n",i);
                        send(ssx[i].ss_socket,&FilenameAndOperation,sizeof(fileNameAndOperation),0);
                        printf("Sent File or folder name and Operation number successfully\n");
                    ssx[i].num_of_folders++;
                    saveFolderCounts(ssx); // Save the updated file counts
                    break;
                }
            }
            if(i==number_of_storage_servers) {
                // Create a new storage server if all are full
                number_of_storage_servers++;
                //CreateNewStorageServer(ssx); // Adjust this function to properly add a new storage server
                // Create the file or folder in the newly created storage server
                // Your logic to connect and create a file or folder in the new storage server
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
                        printf("Connected to the storage server %d.\n\n",number_of_storage_servers-1);
                        send(ssx[number_of_storage_servers-1].ss_socket,&FilenameAndOperation,sizeof(fileNameAndOperation),0);
                        printf("Sent File or folder name and Operation number successfully\n");
                        if(operation_num==4){
                            ssx[i].num_of_files++;
                            saveFileCounts(ssx);
                        }
                        else{
                            ssx[i].num_of_files++;
                            saveFolderCounts(ssx);
                        }
                //saveFileCounts(ssx); // Save the updated file counts
            }
        } 
        else {
            // for(int i=0;i<CURRENT_STORAGE_SERVERS;i++){
            //     char *slash1 = strchr(path, '/');
            //     char *slash2 = strchr(ssx[i].paths_accessible->path, '/');
            //     // Calculate the length up to the first '/'
            //     size_t len1 = (size_t)(slash1 - path);
            //     size_t len2 = (size_t)(slash2 - ssx[i].paths_accessible->path);
            //     char path_dup[len1+1];
            //     char accessible_path_dup[len2+1];
            //     strncpy(path_dup,path,len1);
            //     strncpy(accessible_path_dup,ssx[i].paths_accessible->path,len2);
            //     // Compare strings up to the first '/'
            //     int result = strcmp(path_dup, accessible_path_dup);
            //     if(result==0){
            //         printf("File or folder is in storage server %d",i);
            //         int port=5000+i;
            //         ssx[i].ss_socket = socket(AF_INET, SOCK_STREAM, 0);
            //             if (ssx[i].ss_socket < 0){
            //                 perror("Socket error");
            //                 exit(1);
            //             }
            //             else{
            //                 printf("TCP SS_CLIENT SOCKET CREATED for NS.\n");
            //             }

            //             memset(&ssx[i].ss_address, '\0', sizeof(ssx[i].ss_address));
            //             ssx[i].ss_address.sin_family = AF_INET;
            //             ssx[i].ss_address.sin_port = port;
            //             ssx[i].ss_address.sin_addr.s_addr = inet_addr(ip);

            //             // Connect to the server
            //             connect(ssx[i].ss_socket, (struct sockaddr*)&ssx[i].ss_address, sizeof(ssx[i].ss_address));
            //             printf("Connected to the storage server %d.\n\n",i);
            //         send(ssx[i].ss_socket,&FilenameAndOperation,sizeof(fileNameAndOperation),0);
            //         printf("Sent File or folder name and Operation number successfully\n");
            //         if(operation_num==4){
            //             ssx[i].num_of_files++;
            //         }
            //         else{
            //             ssx[i].num_of_folders++;
            //         }
            //         break;
            //     }
            //     else{
            //         printf("Path not found\n");
            //         printf("Creation not successful since path not found\n");
            //         strcpy(message_status.status_message,"Creation successful since path not found");
            //         //send(nm_as_client_socket,&message_status,MAX_LENGTH,0);
            //     }
            // }
            // Logic to find the appropriate storage server for the path and create there
        }
    } else if (operation_num == 3 || operation_num == 8) {
        // Logic for deletion operations
    }
    // Other operations handling
}

// void CreateandDeleteOperation(SS_Info ssx[MAX_STORAGE_SERVERS], struct acknowledgmentMessage message_status,fileNameAndOperation FilenameAndOperation)
// {
//     int operation_num=FilenameAndOperation.operation_number;
//     char path[20];
//     strcpy(path,FilenameAndOperation.name_of_file_or_folder);
//     loadFileCounts(ssx);
//     if(operation_num==4 || operation_num==9){
//         if(strchr(path, '/')==NULL){                  //single file or directory
//             int i;//if(1){ int i;                       //condition for storge server being not full
//                 for(i=0;i<number_of_storage_servers;i++){
//                     printf("Num of files in %d ss: %d",i,ssx[i].num_of_files);
//                     if(operation_num==4){
//                         printf("Num of files in %d ss: %d",i,ssx[i].num_of_files);
//                         if(ssx[i].num_of_files==MAX_FILE_LIMIT){
//                             continue;
//                         }
//                         int port=5000+i;
//                         ssx[i].ss_socket = socket(AF_INET, SOCK_STREAM, 0);
//                         if (ssx[i].ss_socket < 0){
//                             perror("Socket error");
//                             exit(1);
//                         }
//                         else{
//                             printf("TCP SS_CLIENT SOCKET CREATED for NS.\n");
//                         }

//                         memset(&ssx[i].ss_address, '\0', sizeof(ssx[i].ss_address));
//                         ssx[i].ss_address.sin_family = AF_INET;
//                         ssx[i].ss_address.sin_port = port;
//                         ssx[i].ss_address.sin_addr.s_addr = inet_addr(ip);

//                         // Connect to the server
//                         connect(ssx[i].ss_socket, (struct sockaddr*)&ssx[i].ss_address, sizeof(ssx[i].ss_address));
//                         printf("Connected to the storage server %d.\n\n",i);
//                         send(ssx[i].ss_socket,&FilenameAndOperation,sizeof(fileNameAndOperation),0);
//                         printf("Sent File or folder name and Operation number successfully\n");
//                         ssx[i].num_of_files++;
//                         printf("Num of files in %d ss: %d\n",i,ssx[i].num_of_files);
//                         break;
//                     }
//                     else{
//                         if(ssx[i].num_of_folders==MAX_FOLDER_LIMIT){
//                             continue;
//                         }
//                         int port=5000+i;
//                         ssx[i].ss_socket = socket(AF_INET, SOCK_STREAM, 0);
//                         if (ssx[i].ss_socket < 0){
//                             perror("Socket error");
//                             exit(1);
//                         }
//                         else{
//                             printf("TCP SS_CLIENT SOCKET CREATED for NS.\n");
//                         }

//                         memset(&ssx[i].ss_address, '\0', sizeof(ssx[i].ss_address));
//                         ssx[i].ss_address.sin_family = AF_INET;
//                         ssx[i].ss_address.sin_port = port;
//                         ssx[i].ss_address.sin_addr.s_addr = inet_addr(ip);

//                         // Connect to the server
//                         connect(ssx[i].ss_socket, (struct sockaddr*)&ssx[i].ss_address, sizeof(ssx[i].ss_address));
//                         printf("Connected to the storage server %d.\n\n",i);
//                         send(ssx[i].ss_socket,&FilenameAndOperation,sizeof(fileNameAndOperation),0);
//                         printf("Sent File or folder name and Operation number successfully\n");
//                         ssx[i].num_of_folders++;
//                         break;
//                     }
//                 }
//             //}
//             //else{
//             if(i==number_of_storage_servers){
//                 number_of_storage_servers++;
//                 //create new storage server
//                 SS_Info* ss=CreateNewStorageServer(ssx);
//                 int port=5000+number_of_storage_servers-1;
//                         ss[number_of_storage_servers-1].ss_socket = socket(AF_INET, SOCK_STREAM, 0);
//                         if (ssx[number_of_storage_servers-1].ss_socket < 0){
//                             perror("Socket error");
//                             exit(1);
//                         }
//                         else{
//                             printf("TCP SS_CLIENT SOCKET CREATED for NS.\n");
//                         }

//                         memset(&ssx[number_of_storage_servers-1].ss_address, '\0', sizeof(ssx[number_of_storage_servers-1].ss_address));
//                         ssx[number_of_storage_servers-1].ss_address.sin_family = AF_INET;
//                         ssx[number_of_storage_servers-1].ss_address.sin_port = port;
//                         ssx[number_of_storage_servers-1].ss_address.sin_addr.s_addr = inet_addr(ip);

//                         // Connect to the server
//                         connect(ssx[number_of_storage_servers-1].ss_socket, (struct sockaddr*)&ssx[number_of_storage_servers-1].ss_address, sizeof(ssx[number_of_storage_servers-1].ss_address));
//                         printf("Connected to the storage server %d.\n\n",number_of_storage_servers-1);
//                         send(ssx[number_of_storage_servers-1].ss_socket,&FilenameAndOperation,sizeof(fileNameAndOperation),0);
//                         printf("Sent File or folder name and Operation number successfully\n");
//                         if(operation_num==4){
//                             ssx[number_of_storage_servers-1].num_of_files++;
//                         }
//                         else{
//                             ssx[number_of_storage_servers-1].num_of_folders++;
//                         }
//             }
//             //}
//         }
//         else{
//             for(int i=0;i<CURRENT_STORAGE_SERVERS;i++){
//                 char *slash1 = strchr(path, '/');
//                 char *slash2 = strchr(ssx[i].paths_accessible->path, '/');
//                 // Calculate the length up to the first '/'
//                 size_t len1 = (size_t)(slash1 - path);
//                 size_t len2 = (size_t)(slash2 - ssx[i].paths_accessible->path);
//                 char path_dup[len1+1];
//                 char accessible_path_dup[len2+1];
//                 strncpy(path_dup,path,len1);
//                 strncpy(accessible_path_dup,ssx[i].paths_accessible->path,len2);
//                 // Compare strings up to the first '/'
//                 int result = strcmp(path_dup, accessible_path_dup);
//                 if(result==0){
//                     printf("File or folder is in storage server %d",i);
//                     int port=5000+i;
//                     ssx[i].ss_socket = socket(AF_INET, SOCK_STREAM, 0);
//                         if (ssx[i].ss_socket < 0){
//                             perror("Socket error");
//                             exit(1);
//                         }
//                         else{
//                             printf("TCP SS_CLIENT SOCKET CREATED for NS.\n");
//                         }

//                         memset(&ssx[i].ss_address, '\0', sizeof(ssx[i].ss_address));
//                         ssx[i].ss_address.sin_family = AF_INET;
//                         ssx[i].ss_address.sin_port = port;
//                         ssx[i].ss_address.sin_addr.s_addr = inet_addr(ip);

//                         // Connect to the server
//                         connect(ssx[i].ss_socket, (struct sockaddr*)&ssx[i].ss_address, sizeof(ssx[i].ss_address));
//                         printf("Connected to the storage server %d.\n\n",i);
//                     send(ssx[i].ss_socket,&FilenameAndOperation,sizeof(fileNameAndOperation),0);
//                     printf("Sent File or folder name and Operation number successfully\n");
//                     if(operation_num==4){
//                         ssx[i].num_of_files++;
//                     }
//                     else{
//                         ssx[i].num_of_folders++;
//                     }
//                     break;
//                 }
//                 else{
//                     printf("Path not found\n");
//                     printf("Creation not successful since path not found\n");
//                     strcpy(message_status.status_message,"Creation successful since path not found");
//                     //send(nm_as_client_socket,&message_status,MAX_LENGTH,0);
//                 }
//             }
//         }
//     }
//     else if(operation_num==3 || operation_num==8){
//         if(strchr(path,'/')==NULL){
//             for(int i=0;i<number_of_storage_servers;i++){
                
//             }
//         }
//     }
    
// }

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

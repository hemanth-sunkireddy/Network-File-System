#include "headers.h"


void duplicating_create_and_delete(fileNameAndOperation FilenameAndOperation, int port_of_duplicated_server){

                        int temp_socket_to_duplicate; 
                        temp_socket_to_duplicate = socket(AF_INET, SOCK_STREAM, 0);
                        if (temp_socket_to_duplicate < 0){
                            perror("Socket error");
                            exit(1);
                        }
                        else{
                            printf("TCP SS_CLIENT SOCKET CREATED for NS.\n");
                        }
                        struct sockaddr_in client_address_of_duplicate;
                        memset(&client_address_of_duplicate, '\0', sizeof(client_address_of_duplicate));
                        client_address_of_duplicate.sin_family = AF_INET;
                        client_address_of_duplicate.sin_port = port_of_duplicated_server;
                        client_address_of_duplicate.sin_addr.s_addr = inet_addr(ip);
                        // Connect to the server
                        connect(temp_socket_to_duplicate, (struct sockaddr*)&client_address_of_duplicate, sizeof(client_address_of_duplicate));
                        send(temp_socket_to_duplicate,&FilenameAndOperation,sizeof(fileNameAndOperation),0);
}

void CreateandDeleteOperation(SS_Info ssx[MAX_STORAGE_SERVERS], acknowledgmentMessage message_status, fileNameAndOperation FilenameAndOperation, int* storage_server_connection_socket) {
    int operation_num = FilenameAndOperation.operation_number;
    char path[MAX_PATH_LENGTH];
    strcpy(path, FilenameAndOperation.name_of_file_or_folder);
    //FindNumberOfStorageServers(ssx);
    printf("Num of storage servers: %d\n",number_of_storage_servers);
    // Load the file counts at the beginning of the program
    loadFileCounts(ssx);
    loadFolderCounts(ssx);
    if(operation_num==4 || operation_num==9){ // Create file or folder
        if (strchr(path, '/') == NULL) { // Check if it's a single file or directory
            int i;
            for (i = 0; i < MAX_STORAGE_SERVERS; i++) {
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
                        printf("%d %d %d %d\n",ssx[i].SS_port,ssx[i].storage_server_number,ssx[i].num_of_files,ssx[i].num_of_folders);
                        // Connect to the server
                        connect(ssx[i].ss_socket, (struct sockaddr*)&ssx[i].ss_address, sizeof(ssx[i].ss_address));
                        printf("Connected to the storage server %d.\n\n",i);
                        printf("%d %d %d %d\n",ssx[i].SS_port,ssx[i].storage_server_number,ssx[i].num_of_files,ssx[i].num_of_folders);
                        send(ssx[i].ss_socket,&FilenameAndOperation,sizeof(fileNameAndOperation),0);
                        printf("Sent File or folder name and Operation number successfully\n");

                        // Adding the naming server acted as client to the storage server socket to the storage_server_connected_socket.
                        *storage_server_connection_socket = ssx[i].ss_socket;
                        // Just checking status message from storage server. 
                        // char message_from_ss[MAX_LENGTH];
                        // recv(ssx[i].ss_socket, message_from_ss, MAX_LENGTH, 0);
                        // printf("MESSAGE FROM STORAGE SERVER: %s\n", message_from_ss);

                        strcpy(ssx[i].paths_accessible[ssx[i].num_of_files+ssx[i].num_of_folders].path,path); 
                        printf("Current stored path: %s",ssx[i].paths_accessible[ssx[i].num_of_files+ssx[i].num_of_folders].path);
                    ssx[i].num_of_files++;
                    printf("%d\n",ssx[i].num_of_files);
                    saveFileCounts(ssx); // Save the updated file 
                    //savePathsAccessible(ssx); 

                    // Duplicating the creation of file in the respective duplicated servers. 
                        int port_of_duplicated_server1 = 10 + port;
                        int port_of_duplicated_server2 = 20 + port; 
                        duplicating_create_and_delete(FilenameAndOperation, port_of_duplicated_server1);
                        duplicating_create_and_delete(FilenameAndOperation, port_of_duplicated_server2);

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
                        strcpy(ssx[i].paths_accessible[ssx[i].num_of_folders+ssx[i].num_of_files].path,path);
                        printf("Current stored path: %s",ssx[i].paths_accessible[ssx[i].num_of_files+ssx[i].num_of_folders].path);
                    ssx[i].num_of_folders++;
                    saveFolderCounts(ssx); // Save the updated file counts
                    *storage_server_connection_socket = ssx[i].ss_socket;
                    //savePathsAccessible(ssx); 
                    break;
                }
            }
            printf("First line check condition.\n");
            if(i==number_of_storage_servers) {
                printf("Second line check condition.\n");
                // Create a new storage server if all are full
                number_of_storage_servers++;
                printf("%d\n",number_of_storage_servers);
                //CreateNewStorageServer(ssx); // Adjust this function to properly add a new storage server
                // Create the file or folder in the newly created storage server
                // Your logic to connect and create a file or folder in the new storage server
                //CreateNewStorageServer(ssx);
                printf("%d %d %d %d\n",ssx[i].SS_port,ssx[i].storage_server_number,ssx[i].num_of_files,ssx[i].num_of_folders);
                int port=ssx[i].SS_port;
                        ssx[number_of_storage_servers-1].ss_socket = socket(AF_INET, SOCK_STREAM, 0);
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
                        printf("hello\n");
                        // Connect to the server
                        connect(ssx[number_of_storage_servers-1].ss_socket, (struct sockaddr*)&ssx[number_of_storage_servers-1].ss_address, sizeof(ssx[number_of_storage_servers-1].ss_address));
                        printf("Connected to the storage server %d.\n\n",number_of_storage_servers-1);
                        printf("hi");
                        send(ssx[number_of_storage_servers-1].ss_socket,&FilenameAndOperation,sizeof(fileNameAndOperation),0);
                        printf("Sent File or folder name and Operation number successfully\n");
                        if(operation_num==4){
                            strcpy(ssx[i].paths_accessible[ssx[i].num_of_files+ssx[i].num_of_folders].path,path);
                            ssx[i].num_of_files++;
                            saveFileCounts(ssx);
                        }
                        else{
                            strcpy(ssx[i].paths_accessible[ssx[i].num_of_folders+ssx[i].num_of_files].path,path);
                            ssx[i].num_of_folders++;
                            saveFolderCounts(ssx);
                        }
                        printf("Current stored path: %s",ssx[i].paths_accessible[ssx[i].num_of_files+ssx[i].num_of_folders].path);
                        *storage_server_connection_socket = ssx[i].ss_socket;
                //saveFileCounts(ssx); // Save the updated file counts
            }
        } 
        else {
            printf("Thid status check here.\n");
            for (int i = 0; i < MAX_STORAGE_SERVERS; i++) {
            for (int j = 0; j < ssx[i].num_of_files+ssx[i].num_of_folders; j++) {
            // Check if the path matches any stored file or folder path
            if (strncmp(path,ssx[i].paths_accessible[j].path,strlen(ssx[i].paths_accessible[j].path)) == 0 && path[strlen(ssx[i].paths_accessible[j].path)]=='/' && path[strlen(ssx[i].paths_accessible[j].path) + 1] != '\0') {
                printf("File or folder found in storage server %d\n", i);
                int port = 5000 + i;
                ssx[i].ss_socket = socket(AF_INET, SOCK_STREAM, 0);
                if (ssx[i].ss_socket < 0) {
                    perror("Socket error");
                    exit(1);
                } else {
                    printf("TCP SS_CLIENT SOCKET CREATED for NS.\n");
                }

                memset(&ssx[i].ss_address, '\0', sizeof(ssx[i].ss_address));
                ssx[i].ss_address.sin_family = AF_INET;
                ssx[i].ss_address.sin_port = port;
                ssx[i].ss_address.sin_addr.s_addr = inet_addr(ip);

                // Connect to the server
                connect(ssx[i].ss_socket, (struct sockaddr*)&ssx[i].ss_address, sizeof(ssx[i].ss_address));
                printf("Connected to the storage server %d.\n\n", i);
                send(ssx[i].ss_socket, &FilenameAndOperation, sizeof(fileNameAndOperation), 0);
                printf("Sent File or folder name and Operation number successfully\n");
                *storage_server_connection_socket = ssx[i].ss_socket;
                break;
            }
        }
            }
            // Logic to find the appropriate storage server for the path and create there
        }
    } 
    else if (operation_num == 3 || operation_num == 8) {
        for(int i=0;i<MAX_STORAGE_SERVERS;i++){
            //if(operation_num==3){
                for(int j=0;j<ssx[i].num_of_files+ssx[i].num_of_folders;j++){
                    if((strncmp(path,ssx[i].paths_accessible[j].path,strlen(ssx[i].paths_accessible[j].path))==0 && path[strlen(ssx[i].paths_accessible[j].path)]=='\0') || (strncmp(path,ssx[i].paths_accessible[j].path,strlen(ssx[i].paths_accessible[j].path))==0 && path[strlen(ssx[i].paths_accessible[j].path)]=='/' && path[strlen(ssx[i].paths_accessible[j].path) + 1] != '\0')){
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
                        //printf("%d %d %d %d\n",ssx[i].SS_port,ssx[i].storage_server_number,ssx[i].num_of_files,ssx[i].num_of_folders);
                        // Connect to the server
                        connect(ssx[i].ss_socket, (struct sockaddr*)&ssx[i].ss_address, sizeof(ssx[i].ss_address));
                        printf("Connected to the storage server %d.\n\n",i);
                        send(ssx[i].ss_socket,&FilenameAndOperation,sizeof(fileNameAndOperation),0);
                        printf("Sent File or folder name and Operation number successfully\n");
                        if((strncmp(path,ssx[i].paths_accessible[j].path,strlen(ssx[i].paths_accessible[j].path))==0 && path[strlen(ssx[i].paths_accessible[j].path)]=='\0')){
                            char str[MAX_PATH_LENGTH];
                            strcpy(str,ssx[i].paths_accessible[ssx[i].num_of_files+ssx[i].num_of_folders].path);
                            if(operation_num==3){
                                ssx[i].num_of_files--;
                                printf("%d\n",ssx[i].num_of_files);
                                saveFileCounts(ssx);
                            }
                            else{
                                ssx[i].num_of_folders--;
                                printf("%d\n",ssx[i].num_of_folders);
                                saveFolderCounts(ssx);
                            }
                            for(int k=j;k<ssx[i].num_of_files+ssx[i].num_of_folders;k++){
                                strcpy(ssx[i].paths_accessible[k].path,ssx[i].paths_accessible[k+1].path);
                            }
                            strcpy(ssx[i].paths_accessible[ssx[i].num_of_files+ssx[i].num_of_folders].path,str);
                            }
                        *storage_server_connection_socket = ssx[i].ss_socket;
                    }
                }
        }
    }
}

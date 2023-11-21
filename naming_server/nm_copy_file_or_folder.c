#include "headers.h"

void CopyFileorFolderOperations(SS_Info ssx[MAX_STORAGE_SERVERS], acknowledgmentMessage message_status, fileNameAndOperation FilenameAndOperation, Copy_source_dest *copy_src_dest_oper, int* storage_server_connection_socket)
{
    if(FilenameAndOperation.operation_number == 11){
        char dest_path[MAX_PATH_LENGTH];
        int dest_ss_port;
        strcpy(dest_path,copy_src_dest_oper->destination_path);
        int i;
        for(i=0;i<MAX_STORAGE_SERVERS;i++){
            int count=0;
            for(int j=0;j<ssx[i].num_of_files+ssx[i].num_of_folders;j++){
                if((strncmp(dest_path,ssx[i].paths_accessible[j].path,strlen(ssx[i].paths_accessible[j].path))==0 && dest_path[strlen(ssx[i].paths_accessible[j].path)]=='\0') || (strncmp(dest_path,ssx[i].paths_accessible[j].path,strlen(ssx[i].paths_accessible[j].path))==0 && dest_path[strlen(ssx[i].paths_accessible[j].path)]=='/' && dest_path[strlen(ssx[i].paths_accessible[j].path) + 1] != '\0')){
                    dest_ss_port=5000+i;
                    count++;
                    break;
                }
            }
            if(count==1){
                break;
            }
        }
        int dest_ss_no=i;
        printf("Destination port: %d\n",dest_ss_port);
        printf("Destination ss_no: %d\n",dest_ss_no);
        char src_path[MAX_PATH_LENGTH];
        int src_ss_port;
        strcpy(src_path,copy_src_dest_oper->source_path);
        for(int i=0;i<MAX_STORAGE_SERVERS;i++){
                int temp_count = 0; 
                for(int j=0;j<ssx[i].num_of_files+ssx[i].num_of_folders;j++){
                    if((strncmp(src_path,ssx[i].paths_accessible[j].path,strlen(ssx[i].paths_accessible[j].path))==0 && src_path[strlen(ssx[i].paths_accessible[j].path)]=='\0') || (strncmp(src_path,ssx[i].paths_accessible[j].path,strlen(ssx[i].paths_accessible[j].path))==0 && src_path[strlen(ssx[i].paths_accessible[j].path)]=='/' && src_path[strlen(ssx[i].paths_accessible[j].path) + 1] != '\0')){
                        src_ss_port=5000+i;
                        printf("Source port: %d\n",src_ss_port);
                        temp_count++;
                    }
                }
                if ( temp_count == 1 ){
                    break;
                }
        }
        printf("SOURCE STORAGE PORT: %d, Destination storage port:%d\n", src_ss_port, dest_ss_port);

            // Appending the details of the src and dest ss details to the struct. 
            Copy_source_dest data_of_the_source_and_destination_storage_servers; 
            data_of_the_source_and_destination_storage_servers.operation_number = 11; 
            strcpy(data_of_the_source_and_destination_storage_servers.destination_path , dest_path);
            data_of_the_source_and_destination_storage_servers.source_or_destination_storage_server = 0;
            strcpy(data_of_the_source_and_destination_storage_servers.source_path, src_path);
            data_of_the_source_and_destination_storage_servers.port_number = src_ss_port;
            int src_ss_socket = socket(AF_INET, SOCK_STREAM, 0);
            struct sockaddr_in src_ss_address;
            socklen_t src_ss_address_size;
            if (src_ss_socket < 0){
                perror("Socket error");
                exit(1);
            }
            else{
                printf("TCP SS_CLIENT SOCKET CREATED for NS.\n");
            }
            memset(&src_ss_address, '\0', sizeof(src_ss_address));
            src_ss_address.sin_family = AF_INET;
            src_ss_address.sin_port = src_ss_port;
            src_ss_address.sin_addr.s_addr = inet_addr(ip);
            // Connect to the server
            connect(src_ss_socket, (struct sockaddr*)&src_ss_address, sizeof(src_ss_address));
            printf("Connected to the source storage server: %d.\n", src_ss_port);
            send(src_ss_socket,&FilenameAndOperation,MAX_LENGTH,0);
            printf("Initially sent the details of operation number in FilenameAndOperation to the source as well as destination server.\n");
            int sending_status=send(src_ss_socket,&data_of_the_source_and_destination_storage_servers,MAX_LENGTH,0);
            printf("Sending status: %d\n",sending_status);
            printf("Sent source, dest SS details to the storage server.\n");
            
            char source_ss_number[MAX_LENGTH];
            sprintf(source_ss_number, "%d", src_ss_port);
            // Now sending the storage server port number to the source storage server. 
            int sending_status_of_the_port_of_storage_server = send(src_ss_socket, source_ss_number, MAX_LENGTH, 0);
            printf("Sending stats of the source storage server port number: %d\n", sending_status_of_the_port_of_storage_server);
            // Assigning source storage server to the last recieving status of the naming server in the sending function call. 
            *storage_server_connection_socket = src_ss_socket;

            // Receiving the response from the source storage server. 
            char source_file_data[MAX_LENGTH];
            int receiving_status_from_source_storage_server = recv(src_ss_socket,source_file_data,  MAX_LENGTH,0 );
            printf("Data recieved from the source storage server: %s\n", source_file_data);
            close (src_ss_socket);

            // Now connnecting to the destination storage server to create the file and copy the contents of the file to the destination location. 
            int destination_server_socket = socket(AF_INET, SOCK_STREAM, 0);
            struct sockaddr_in destination_ss_address;
            socklen_t destination_address_size;
            if (src_ss_socket < 0){
                perror("Socket error");
                exit(1);
            }
            else{
                printf("TCP SS_CLIENT SOCKET CREATED for NS.\n");
            }
            memset(&src_ss_address, '\0', sizeof(src_ss_address));
            destination_ss_address.sin_family = AF_INET;
            destination_ss_address.sin_port = dest_ss_port;
            destination_ss_address.sin_addr.s_addr = inet_addr(ip);
            // Connect to the server
            connect(destination_server_socket, (struct sockaddr*)&destination_ss_address, sizeof(destination_ss_address));
            printf("Connected to the source storage server: %d.\n", dest_ss_port);
            send(destination_server_socket,&FilenameAndOperation,MAX_LENGTH,0);
            printf("Initially sent the details of operation number in FilenameAndOperation to the source as well as destination server to the destination storage server.\n");

            // Now making the destination storage server activation for copying the file.  and also copying the file content to the struct.
            data_of_the_source_and_destination_storage_servers.source_or_destination_storage_server = 1;
            strcpy(data_of_the_source_and_destination_storage_servers.file_content, source_file_data);
            data_of_the_source_and_destination_storage_servers.port_number = dest_ss_port; 
            printf("Destination storage server port number: %d\n", data_of_the_source_and_destination_storage_servers.port_number);
            int sending_status_of_destination_ss=send(destination_server_socket,&data_of_the_source_and_destination_storage_servers,MAX_LENGTH,0);


            // Now sending the destination storage server port number. 
            char destination_ss_port_no[MAX_LENGTH];
            sprintf(destination_ss_port_no, "%d", dest_ss_port);
            int sending_status_of_destination_port_number = send(destination_server_socket, destination_ss_port_no, MAX_LENGTH, 0);

            printf("Sending status: %d\n",sending_status_of_destination_ss);
            printf("Sent source, dest SS details to the destination storage server.\n");

    }
    else if(FilenameAndOperation.operation_number==10){
        char dest_path[MAX_PATH_LENGTH];
        int dest_ss_port;
        strcpy(dest_path,copy_src_dest_oper->destination_path);
        int i;
        for(i=0;i<MAX_STORAGE_SERVERS;i++){
            int count=0;
            for(int j=0;j<ssx[i].num_of_files+ssx[i].num_of_folders;j++){
                if((strncmp(dest_path,ssx[i].paths_accessible[j].path,strlen(ssx[i].paths_accessible[j].path))==0 && dest_path[strlen(ssx[i].paths_accessible[j].path)]=='\0') || (strncmp(dest_path,ssx[i].paths_accessible[j].path,strlen(ssx[i].paths_accessible[j].path))==0 && dest_path[strlen(ssx[i].paths_accessible[j].path)]=='/' && dest_path[strlen(ssx[i].paths_accessible[j].path) + 1] != '\0')){
                    dest_ss_port=5000+i;
                    count++;
                    break;
                }
            }
            if(count==1){
                break;
            }
        }
        int dest_ss_no=i;
        printf("Destination port: %d\n",dest_ss_port);
        printf("Destination ss_no: %d\n",dest_ss_no);
        char src_path[MAX_PATH_LENGTH];
        int src_ss_port;
        strcpy(src_path,copy_src_dest_oper->source_path);
        for(int i=0;i<MAX_STORAGE_SERVERS;i++){
                int temp_count=0; 
                for(int j=0;j<ssx[i].num_of_files+ssx[i].num_of_folders;j++){
                    if((strncmp(src_path,ssx[i].paths_accessible[j].path,strlen(ssx[i].paths_accessible[j].path))==0 && src_path[strlen(ssx[i].paths_accessible[j].path)]=='\0') || (strncmp(src_path,ssx[i].paths_accessible[j].path,strlen(ssx[i].paths_accessible[j].path))==0 && src_path[strlen(ssx[i].paths_accessible[j].path)]=='/' && src_path[strlen(ssx[i].paths_accessible[j].path) + 1] != '\0')){
                        src_ss_port=5000+i;
                        printf("Source port: %d\n",src_ss_port);
                        temp_count++;
                    }
                }
                if(temp_count==1){
                    break;
                }
        }
        int src_ss_no=i;
        char source_dir[5*MAX_LENGTH];
        strcpy(source_dir,"../storage_server/SS");
        char source_ss_no_in_char[5];
        sprintf(source_ss_no_in_char, "%d",src_ss_no+1);
        strcat(source_dir,source_ss_no_in_char);
        strcat(source_dir,"/");
        strcat(source_dir,src_path);

        printf("Source path after final addition: %s\n", source_dir);



        char destination_path[5 * MAX_LENGTH];
        strcpy(destination_path,"../storage_server/SS");
        char destination_ss_no_in_char[5];
        sprintf(destination_ss_no_in_char, "%d",  dest_ss_no + 1 ) ; 
        strcat(destination_path, destination_ss_no_in_char);
        strcat(destination_path, "/");
        strcat(destination_path, dest_path);

        printf("Destination path final addition: %s\n", destination_path);


        // Concatenate the command string
        char command[10 * MAX_LENGTH];
        sprintf(command, "cp -R %s %s", source_dir, destination_path);

        // Execute the command
        system(command);

    }
}
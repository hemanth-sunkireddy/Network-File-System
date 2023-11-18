#include "headers.h"

void client_receving_data(int client_socket,fileNameAndOperation file_or_folder_details){

    // Acknowledgment from the storage server. 
    if ( file_or_folder_details.operation_number == 1){
        acknowledgmentMessage message_status;  
        char data_of_file[MAX_LENGTH];         
        int bytes_received = recv(client_socket, &message_status, MAX_LENGTH, 0);
        int bytes_second_received = recv(client_socket, data_of_file, MAX_LENGTH, 0);

        
        printf("Status of the operation: %s\n", message_status.status_message);
        printf("Data of the file: %s\n", data_of_file);

        // int length;
        // sscanf(buffer, "%d", &length);

        // printf("Length: %d\n", length);

        // for (int i = 0; i < length; i++){
        //     bytes_received = recv(client_socket, buffer, MAX_LENGTH, 0);

        //     buffer[bytes_received] = '\0';

        //     printf("%s", buffer);
        // }

        printf("\nRead response successfully.\n");
    }
    if ( file_or_folder_details.operation_number == 2){
        //writing
        char data_need_to_send_to_ss[MAX_LENGTH];
        printf("Enter the data to write in the file: ");

        // Only string that contains without spaces now sending later will implement with spaces also. 
        scanf("%s", data_need_to_send_to_ss);
        printf("Data need to write in file: %s\n", data_need_to_send_to_ss);

        if(send(client_socket, data_need_to_send_to_ss, MAX_LENGTH, 0) != -1){
            printf("Data send successfully\n");
        }
        else{
            printf("Error in send the data to the storage server.\n");
        }
        
        char status_recieved_from_storage_server[MAX_LENGTH];
        if ( recv(client_socket, status_recieved_from_storage_server, MAX_LENGTH, 0) != -1 ){
            printf("status of operation: %s\n", status_recieved_from_storage_server);
        }
        else{
            printf("Error in recieving status message from storage server.\n");
        }
                

    }
    if ( file_or_folder_details.operation_number == 5){
         acknowledgmentMessage message_status;
        if(recv(client_socket , &message_status , MAX_LENGTH ,0)==-1){
            printf("error in getting folder data\n");
        }
        printf("Status of the operation: %s\n", message_status.status_message);
        fileInformation information_of_file;
        if(recv(client_socket , &information_of_file , MAX_LENGTH ,0)==-1){
            printf("error in getting folder data\n");
        }
        printf("File Group: %d\n",information_of_file.file_group);
        printf("File inode number: %u\n",information_of_file.file_inode_number);
        printf("File owner: %d\n",information_of_file.file_owner);
        printf("File size: %ld\n",information_of_file.file_size);


    }
    if ( file_or_folder_details.operation_number == 6){
        //folder inf 
        acknowledgmentMessage message_status;
        if(recv(client_socket,&message_status,sizeof(message_status),0)==-1){
            printf("error in getting folder data\n");
        }
        
        printf("Folder name: %s\n ",message_status.folder_information.folder_name);
        printf("Size: %d\n ",message_status.folder_information.size);
        printf("Permissions: %s\n ",message_status.folder_information.Permissions);
        printf("Last Modified Time: %s\n ",message_status.folder_information.last_modified_time);
        printf("created Time: %s\n ",message_status.folder_information.created_time);
        printf("Parent Folder: %s\n ",message_status.folder_information.parent_folder);
        printf("Total Size in KB: %f\n ",message_status.folder_information.total_size_of_folder_in_KB);
        printf("Total Files: %d\n ",message_status.folder_information.total_files);
        printf("Total Folders: %d\n ",message_status.folder_information.total_folders);



    }
    if ( file_or_folder_details.operation_number == 7){
        //listing file or folder
        acknowledgmentMessage message_status;
        if(recv(client_socket,&message_status,sizeof(message_status),0)==-1){
            printf("error in getting folder data\n");
        }

        printf("Total Contents %d\n",message_status.list_of_folder.total_number_of_files_or_folders);
        for(int i=0;i<message_status.list_of_folder.total_number_of_files_or_folders;i++){
            printf("%d)  %s\n",i+1,message_status.list_of_folder.names_of_file_or_folder[i].file_or_folder_name);
        }

    }






}

#include "headers.h"

void send_server_request(acknowledgmentMessage message_status, int client_socket)
{
    if ( message_status.operation_number == 1){
        char length_of_string[MAX_LENGTH];
        printf("%d\n", message_status.file_or_folder_content[0].total_chunks);
        sprintf(length_of_string, "%d", message_status.file_or_folder_content[0].total_chunks);
        
        // Send the length of the data as a separate message.
        // send(client_socket, length_of_string, MAX_LENGTH, 0);
        // printf("Length of the packet sent successfully\n");
        
        // for (int i = 0; i < message_status.file_or_folder_content[0].total_chunks; i++) 
        // {
        //     printf("%d chunk data: %s\n", message_status.file_or_folder_content[i].data);
        //     send(client_socket, message_status.file_or_folder_content[i].data, MAX_LENGTH, 0);
        // }
        

        printf("File data: %s\n", message_status.file_or_folder_content[0].data);
        send(client_socket, &message_status, MAX_LENGTH, 0);
        send(client_socket, message_status.file_or_folder_content[0].data, MAX_LENGTH, 0);
        printf("Response for reading a file sent successfully\n");
    }
    else if ( message_status.operation_number == 2){
        send(client_socket, message_status.status_message, MAX_LENGTH, 0);
        printf("Response of writing a file sent successfully.\n");
    }
    else if ( message_status.operation_number == 3 ){ 
        send(client_socket, message_status.status_message, MAX_LENGTH, 0);
        printf("Response of deleting a file sent successfully.\n");
    }
    else if ( message_status.operation_number == 4 ) { 
        printf("Message to send: %s\n", message_status.status_message);
        int sending_status = send(client_socket, message_status.status_message, MAX_LENGTH, 0);
        printf("Sending status of creation: %d\n", sending_status);
        printf("Response of creating a file sent successfully.\n");
    }
    else if ( message_status.operation_number == 5 ) { 
        printf("File Owner: %d\n", message_status.file_information.file_owner);
        fileInformation information_of_the_file; 

        // Copying the file details in the new struct and passing the struct to avoid data loss.
        information_of_the_file.file_group = message_status.file_information.file_group;
        information_of_the_file.file_inode_number = message_status.file_information.file_inode_number;
        information_of_the_file.file_owner = message_status.file_information.file_owner;
        information_of_the_file.file_size = message_status.file_information.file_size;

        send(client_socket, &message_status, MAX_LENGTH, 0 ); 
        send(client_socket, &information_of_the_file, MAX_LENGTH, 0);
        
        printf("Reponse of getting additional information sent successfully.\n");
    }
    else if ( message_status.operation_number == 6 ){
        
        // Sending Just status Message.
        send(client_socket, message_status.status_message, MAX_LENGTH, 0);  

        // Now sending folder information details. 
        folderInformation information_of_folder; 
        information_of_folder.size = message_status.folder_information.size;
        information_of_folder.total_files = message_status.folder_information.total_files;
        information_of_folder.total_folders = message_status.folder_information.total_folders;  

        send(client_socket, &information_of_folder, MAX_LENGTH * MAX_LENGTH, 0);

        printf("Response of folder information sent successfully.\n");
    }
    else if ( message_status.operation_number == 7 ) { 
        // Sending Just status Message.
        send(client_socket, message_status.status_message, MAX_LENGTH, 0);

        // Now sending list of all folders and files in a given Folder. 
        char total_count_in_char[MAX_LENGTH]; 
        int total_number_of_files_and_folders = message_status.list_of_folder.total_number_of_files_or_folders;
        sprintf(total_count_in_char, "%d", total_number_of_files_and_folders);

        printf("Total count of all files and folders: %s\n", total_count_in_char);
        send(client_socket, total_count_in_char, MAX_LENGTH, 0);

        for (int i = 0; i < total_number_of_files_and_folders; i++ ) { 
            printf("File / Folder: %s\n", message_status.list_of_folder.names_of_file_or_folder[i].file_or_folder_name);
            send(client_socket, message_status.list_of_folder.names_of_file_or_folder[i].file_or_folder_name, MAX_LENGTH, 0);
        }

        printf("Response of listing all files and folders sent successfully.\n");
    }
    else if( message_status.operation_number == 8 ){
        send(client_socket, message_status.status_message, MAX_LENGTH, 0);
        printf("Response of deleting a Folder successfully.\n");
    }
    else if ( message_status.operation_number == 9){
        send(client_socket, message_status.status_message, MAX_LENGTH, 0);
        printf("Response of creating a Folder sent successfully.\n");
    }

}

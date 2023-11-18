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
        send(client_socket, message_status.status_message, MAX_LENGTH, 0 ); 
        printf("Reponse of getting additional information sent successfully.\n");
    }
    else if ( message_status.operation_number == 6 ){
        send(client_socket, message_status.status_message, MAX_LENGTH, 0);    
        send(client_socket, &message_status.folder_information, MAX_LENGTH * MAX_LENGTH, 0);
        printf("Response of folder information sent successfully.\n");
    }
    else if ( message_status.operation_number == 7 ) { 
        send(client_socket, message_status.status_message, MAX_LENGTH, 0);
        printf("Resonse of listing all files and folders sent successfully.\n");
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

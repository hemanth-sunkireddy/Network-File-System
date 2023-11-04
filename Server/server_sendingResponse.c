#include "../headers.h"

void send_server_request(acknowledgmentMessage message_status, int client_socket)
{
    printf("OPERATION NUMBER: %d\n", message_status.operation_number);
    char operation_number[MAX_LENGTH];
    sprintf(operation_number, "%d", message_status.operation_number);
    printf("OPERATION NUMBER IN STRING: %s\n", operation_number);

    // Send the operation number to the client.
    send(client_socket, operation_number, MAX_LENGTH, 0);

    printf("operation status: %s\n", message_status.status_message);

    if ( message_status.operation_number == 1){
        char length_of_string[MAX_LENGTH];
        printf("%d\n", message_status.file_or_folder_content[0].total_chunks);
        sprintf(length_of_string, "%d", message_status.file_or_folder_content[0].total_chunks);
        
        // Send the length of the data as a separate message.
        send(client_socket, length_of_string, MAX_LENGTH, 0);
        printf("Length of the packet sent successfully\n");
        
        for (int i = 0; i < message_status.file_or_folder_content[0].total_chunks; i++) {
            
            send(client_socket, message_status.file_or_folder_content[i].data, MAX_LENGTH, 0);
        }
        
        printf("Response for reading a file sent successfully\n");
    }
    else if ( message_status.operation_number == 4 ) { 
        send(client_socket, message_status.status_message, MAX_LENGTH, 0);
        printf("Response of creating a file sent successfully.\n");
    }
    
    else if ( message_status.operation_number == 9){
        send(client_socket, message_status.status_message, MAX_LENGTH, 0);
        printf("Response of creating a Folder sent successfully.\n");
    }
    else if ( message_status.operation_number == 6 ){
        send(client_socket, message_status.status_message, MAX_LENGTH, 0);    
        send(client_socket, &message_status.folder_information, MAX_LENGTH * MAX_LENGTH, 0);
        printf("Response of folder information sent successfully.\n");
    }

}

#include "../headers.h"

void receive_server_request(int client_socket)
{

    char operation_number_string[MAX_LENGTH];
    recv(client_socket, operation_number_string, MAX_LENGTH, 0);
    int operation_number;
    sscanf(operation_number_string, "%d", &operation_number);
    printf("OPERATION NUMBER: %d\n", operation_number);

    if ( operation_number == 1){
        acknowledgmentMessage message_status;
        char buffer[MAX_LENGTH];
        
        int bytes_received = recv(client_socket, buffer, MAX_LENGTH, 0);

        // Null-terminate the received data.
        buffer[bytes_received] = '\0';

        printf("Received: %s\n", buffer);

        int length;
        sscanf(buffer, "%d", &length);

        printf("Length: %d\n", length);

        for (int i = 0; i < length; i++) {
            bytes_received = recv(client_socket, buffer, MAX_LENGTH, 0);

            buffer[bytes_received] = '\0';

            printf("%s", buffer);
        }

        printf("Read response successfully.\n");
    }
    else if ( operation_number == 2 ) { 
        char message_status[MAX_LENGTH];
        recv(client_socket, message_status, MAX_LENGTH, 0);
        printf("OPERATION STATUS:%s\n", message_status);
    }
    else if ( operation_number == 3 ){
        char message_status[MAX_LENGTH];
        recv(client_socket, message_status, MAX_LENGTH, 0);
        printf("OPERATION STATUS:%s\n", message_status);
    }
    else if ( operation_number == 4 ){
        char message_status[MAX_LENGTH];
        recv(client_socket, message_status, MAX_LENGTH, 0);
        printf("Operation Status: %s\n", message_status);
    }
    else if ( operation_number == 5 ){ 
        char message_status[MAX_LENGTH];
        recv(client_socket, message_status, MAX_LENGTH, 0);
        printf("Operation status:%s\n", message_status);
    }
    else if ( operation_number == 6 ){
        char message_status[MAX_LENGTH];
        recv(client_socket, message_status, MAX_LENGTH, 0);
        printf("Operation status: %s\n", message_status);

        folderInformation folder_details;
        recv(client_socket, &folder_details, MAX_LENGTH * MAX_LENGTH, 0);
        
        printf("Folder Name: %s\n", folder_details.folder_name);
    }
    else if ( operation_number == 8 ){ 
        char message_status[MAX_LENGTH];
        recv(client_socket, message_status, MAX_LENGTH, 0);
        printf("OPERATION STATUS:%s\n", message_status);
    }
    else if (operation_number == 9 ){
        char message_status[MAX_LENGTH];
        recv(client_socket, message_status, MAX_LENGTH, 0);
        printf("%s\n", message_status);
    }

   

}

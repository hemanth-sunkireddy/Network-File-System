#include "headers.h"

acknowledgmentMessage receive_client_request(int client_socket, struct acknowledgmentMessage message_status)
{   
    fileNameAndOperation file_or_folder_details;

    recv(client_socket, &file_or_folder_details, MAX_LENGTH, 0);
    printf("client Asked for this file / folder: %s\n", file_or_folder_details.name_of_file_or_folder);

   

    int operation_number = file_or_folder_details.operation_number; 

    printf("The operation number he had choosen.:%d\n", operation_number);
    
    strcpy(message_status.status_message, "Just checking whether the acknowledgment sent is succesful or not");

    int client_or_naming_server_connected = file_or_folder_details.naming_server_or_client;
    
    if ( client_or_naming_server_connected == 0){
        printf("Client connected not naming server\n");
    }
    else{
        printf("Naming server connected not client\n");
    }

    printf("Storage server number wants to access is: %d\n", file_or_folder_details.storage_server_number);


    

    // Just checking the return status with some copy. 
    strcpy(message_status.status_message, "Just checking whether the details recieved successfully or not.\n");
    
    return message_status;

    
}
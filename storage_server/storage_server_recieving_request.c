#include "headers.h"

acknowledgmentMessage receive_client_request(int client_socket, struct acknowledgmentMessage message_status, int index_of_storage_server)
{   
    fileNameAndOperation file_or_folder_details;

    recv(client_socket, &file_or_folder_details, MAX_LENGTH, 0);
    printf("client/namingServer Asked for this file / folder: %s\n", file_or_folder_details.name_of_file_or_folder);

    int operation_number = file_or_folder_details.operation_number; 

    printf("The operation number he had choosen.:%d\n", operation_number);


    
    char final_path_after_appending_storage_server_number[MAX_LENGTH];
    strcpy(final_path_after_appending_storage_server_number, "SS");

    char operation_number_in_string[MAX_LENGTH];
    sprintf(operation_number_in_string, "%d", index_of_storage_server + 1);

    strcat(final_path_after_appending_storage_server_number, operation_number_in_string);

    char forward_slash_character[2];
    strcpy(forward_slash_character, "/");
    strcat(final_path_after_appending_storage_server_number, forward_slash_character);
    strcat(final_path_after_appending_storage_server_number, file_or_folder_details.name_of_file_or_folder);

    printf("Final path along with server: %s\n", final_path_after_appending_storage_server_number);

    
    if ( operation_number == 3 || operation_number == 4 || operation_number == 8 || operation_number == 9 ) { 
        printf("Storage server connected.\n");
        if ( operation_number == 3 ) { 
            message_status = deleting_the_file(final_path_after_appending_storage_server_number,message_status);
            return message_status;
        }
        else if ( operation_number == 4 ) { 
            message_status = creating_the_file(final_path_after_appending_storage_server_number,message_status);
            return message_status;
        }
        else if ( operation_number == 8 ) { 
            message_status =  deleting_the_folder(final_path_after_appending_storage_server_number, message_status);
            return message_status;
        }
        else if ( operation_number == 9 ) { 
            message_status = creating_the_folder(final_path_after_appending_storage_server_number, message_status);
            return message_status;
        }

    }
    else{
        printf("Client connected.\n");
         // Just checking the return status with the client connections. 
        if ( operation_number == 1 ){
            message_status = reading_the_file(final_path_after_appending_storage_server_number, message_status);
            return message_status;
        }
        else if (operation_number == 6 ){
            message_status = additional_information_of_folder(final_path_after_appending_storage_server_number, message_status);
            return message_status;
        }
        else if (operation_number == 5 ){
            message_status = additional_information_of_file(final_path_after_appending_storage_server_number, message_status);
            return message_status;
        }
        // rest of the operations.
        strcpy(message_status.status_message, "Just sending acknowledgment successful to client connections..\n");
        return message_status;
    }
    
    

   

    
}

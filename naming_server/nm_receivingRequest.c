#include "../headers.h"

acknowledgmentMessage receive_initialized_ss_info(int client_socket, struct acknowledgmentMessage message_status)
{   
    fileNameAndOperation file_or_folder_details;

    recv(client_socket, &file_or_folder_details, MAX_LENGTH, 0);
    printf("client Asked for this file / folder: %s\n", file_or_folder_details.name_of_file_or_folder);

    char full_path[2*MAX_LENGTH];

    // Get the current working directory
    char cwd[MAX_LENGTH];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        snprintf(full_path, 2*MAX_LENGTH, "%s/%s", cwd, file_or_folder_details.name_of_file_or_folder);
    } else {
        perror("Error getting current working directory");
        // Handle the error as needed
    }   

    // int operation_number = file_or_folder_details.operation_number; 

    // // Copying operation_number to the acknowledgment Message struct. 
    // message_status.operation_number = operation_number;

    // printf("The operation number he had choosen.:%d\n", operation_number);
    // if ( operation_number == 1 ) 
    // {
    //     message_status =  reading_the_file(full_path, message_status);   
    //     return message_status;
    // }
    // else if ( operation_number == 2 ) 
    // { 
    //     message_status =  writing_the_file(full_path,message_status);
    //     return message_status;
    // }
    // else if ( operation_number == 3 ) 
    // {
    //     message_status = deleting_the_file(full_path,message_status);
    //     return message_status;
    // }
    // else if ( operation_number == 4 ) 
    // { 
    //     message_status = creating_the_file(full_path,message_status);
    //     return message_status;
    // }
    // else if ( operation_number == 5 ) 
    // { 
    //    message_status =  additional_information_of_file(file_or_folder_details.name_of_file_or_folder,  message_status);
    //    return message_status;
    // }  
    // else if ( operation_number == 6 )
    // { 
    //    message_status =  additional_information_of_folder(file_or_folder_details.name_of_file_or_folder, message_status);
    //    return message_status;
    // }
    // else if ( operation_number == 7 ) 
    // {
    //    message_status =  listing_all_files_and_folders(file_or_folder_details.name_of_file_or_folder, message_status);
    //    return message_status;
    // }
    // else if ( operation_number == 8 ) 
    // { 
    //     message_status =  deleting_the_folder(file_or_folder_details.name_of_file_or_folder, message_status);
    //     return message_status;
    // }
    // else if ( operation_number == 9 )
    // { 
    //     message_status = creating_the_folder(file_or_folder_details.name_of_file_or_folder, message_status);
    //     return message_status;

    // }
    // else if ( operation_number == 8 ) 
    // { 
    //     message_status =  deleting_the_folder(file_or_folder_details.name_of_file_or_folder, message_status);
    //     return message_status;
    // }
    // else if ( operation_number == 9 )
    // { 
    //     message_status = creating_the_folder(file_or_folder_details.name_of_file_or_folder, message_status);
    //     return message_status;
    // }
}
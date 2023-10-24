#include "headers.h"

extern char *message;

void send_client_request(int client_socket){

        fileNameAndOperation file_or_folder_details;

        // Displaying all options in the terminal of the client side.
        printf("Choose one number from below:\n");
        printf("-------------------------------\n");
        printf("1 - Reading a File\n");
        printf("2 - Writing a File\n");
        printf("3 - Deleting a File\n");
        printf("4 - Creating a File\n");
        printf("5 - Getting Additional information of File \n");
        printf("6 - Getting Additional information of Folder\n");
        printf("7 - Writing a Folder\n");
        printf("8 - Deleting a Folder\n");
        printf("9 - Creating a Folder\n");
        printf("10 - Listing all files and Folders\n");
        printf("-------------------------------\n");

        printf("Enter the number:");
        int option_choosing_number; 
        scanf("%d", &file_or_folder_details.operation_number);

        if ( option_choosing_number <= 5){ 
                printf("Enter the file name: ");
        }
        else{
                printf("Enter the Folder name: ");
        }
        scanf("%s", file_or_folder_details.name_of_file_or_folder);
        
        send(client_socket, &file_or_folder_details, MAX_LENGTH, 0);
}

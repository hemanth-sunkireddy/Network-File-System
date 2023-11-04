#include "../headers.h"

acknowledgmentMessage deleting_the_file(char file_name[2*MAX_LENGTH], acknowledgmentMessage message_status){
    char file_path[200];
            if (realpath(file_name, file_path) != NULL) {
                printf("Full file path: %s\n", file_path);
            } else {
                perror("Error getting full path");
                strcpy(message_status.status_message,"ERROR GETTING FULL PATH");
                return message_status; // Return an error code to indicate failure
            }

            if (remove(file_path) == 0) {
                printf("File deleted successfully.\n");
                strcpy(message_status.status_message,"FILE DELETED SUCCESSFULLY");
            } else {
                perror("Error deleting file");
                strcpy(message_status.status_message,"ERROR DELETING FILE");
            }
            //send(client_socket, message, strlen(message), 0);
    return message_status;
}
#include "../headers.h"

acknowledgmentMessage creating_the_file(char file_name[2*MAX_LENGTH], acknowledgmentMessage message_status){

    FILE *file = fopen(file_name, "w");
            
    if (file == NULL) {
        perror("Error creating file");
        strcpy(message_status.status_message, "ERROR CREATING FILE");
        return message_status; // Return an error code to indicate failure
    }
    // Close the file
    fclose(file);
    strcpy(message_status.status_message, "FILE CREATED SUCCESSFULLY");        
    printf("File created successfully: %s\n", file_name);
    return message_status;
}
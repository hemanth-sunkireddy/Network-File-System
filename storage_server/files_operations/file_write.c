#include "../headers.h"

acknowledgmentMessage writing_the_file(char file_name[2*MAX_LENGTH], acknowledgmentMessage message_status, char data_from_client[MAX_LENGTH]){
   
    // Write the string to a file
    FILE *file = fopen(file_name, "w");
    if (file == NULL) {
        perror("Error opening file");
        strcpy(message_status.status_message, "ERROR OPENING FILE");
        return message_status;
    }

    // Write the input string with newline character.
    fprintf(file, "%s\n", data_from_client);

    // Close the file and free the input buffer.
    fclose(file);

    strcpy(message_status.status_message, "WROTE FILE SUCCESSFULLY");
    printf("File write successfully\n");
    return message_status;
}
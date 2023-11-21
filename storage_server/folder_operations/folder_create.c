#include "../headers.h"

acknowledgmentMessage creating_the_folder(char folder_name[MAX_LENGTH], acknowledgmentMessage message_status) {
    //FOR SUBFOLDER YOU NEED TO FIRST MAKE PARENT FOLDER1 THEN FOLDER1/FOLDER2, can also made
    
    int status = mkdir(folder_name, 0755);
    char message[MAX_LENGTH];

    if (status == 0) {
        strcpy(message, "Folder created successfully");
    } else {
        if (errno == EEXIST) {
            strcpy(message, "Folder already exists");
        } else if (errno == EACCES) {
            strcpy(message, "Permission denied to create folder");
        } else if (errno == ENOENT) {
            strcpy(message, "The parent folder does not exist.");
        } else {
            strcpy(message, "Failed to create folder");
        }
    }

    printf("%s", message);
    strcpy ( message_status.status_message, message);
    return message_status;
}
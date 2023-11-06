#include "../headers.h"

acknowledgmentMessage creating_the_folder(char folder_name[MAX_LENGTH], acknowledgmentMessage message_status) {
    //FOR SUBFOLDER YOU NEED TO FIRST MAKE PARENT FOLDER1 THEN FOLDER1/FOLDER2, can also made
    
    int status = mkdir(folder_name, 0755);
    char message[MAX_LENGTH];

    if (status == 0) {
        sprintf(message, "Folder \"%s\" created successfully.\n", folder_name);
    } else {
        if (errno == EEXIST) {
            sprintf(message, "Folder \"%s\" already exists.\n", folder_name);
        } else if (errno == EACCES) {
            sprintf(message, "Permission denied to create folder \"%s\".\n", folder_name);
        } else if (errno == ENOENT) {
            sprintf(message, "The parent folder for \"%s\" does not exist.\n", folder_name);
        } else {
            sprintf(message, "Failed to create folder \"%s\". Error: %s\n", folder_name, strerror(errno));
        }
    }

    printf("%s", message);
    strcpy ( message_status.status_message, message);
    return message_status;
}

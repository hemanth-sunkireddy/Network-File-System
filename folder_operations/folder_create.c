#include "../headers.h"

void creating_the_folder(const char *folderName) {
    //FOR SUBFOLDER
    //YOU NEED TO FIST MAKE PARENT FOLDER1
    //THEN FOLDER1/FOLDER2   ,CAN ASLO MADE
    
    int status = mkdir(folderName, 0755);
    char message[256];

    if (status == 0) {
        sprintf(message, "Folder \"%s\" created successfully.\n", folderName);
    } else {
        if (errno == EEXIST) {
            sprintf(message, "Folder \"%s\" already exists.\n", folderName);
        } else if (errno == EACCES) {
            sprintf(message, "Permission denied to create folder \"%s\".\n", folderName);
        } else if (errno == ENOENT) {
            sprintf(message, "The parent folder for \"%s\" does not exist.\n", folderName);
        } else {
            sprintf(message, "Failed to create folder \"%s\". Error: %s\n", folderName, strerror(errno));
        }
    }

    printf("%s", message);//return message
}

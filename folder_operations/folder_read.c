#include "../headers.h"


acknowledgmentMessage listing_all_files_and_folders(char folder_name[MAX_LENGTH], acknowledgmentMessage message_status){
    printf("Listing of files and folders successful.\n");
    strcpy(message_status.status_message, "Listing of files and folders successful.");


    struct dirent *entry;
    DIR *dir = opendir(folder_name);

    if (dir == NULL) {
        printf("Error opening directory.\n");
    }
    else{

    
    printf("Files and folders in %s:\n", folder_name);

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR) {
            printf("Folder: %s\n", entry->d_name);
        } else {
            printf("File:%s\n", entry->d_name);
        }
    }

    closedir(dir);
    }

    return message_status;
}
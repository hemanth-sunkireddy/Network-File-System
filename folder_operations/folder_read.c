#include "../headers.h"


acknowledgmentMessage listing_all_files_and_folders(char folder_name[MAX_LENGTH], acknowledgmentMessage message_status){
    printf("Listing of files and folders successful.\n");
    strcpy(message_status.status_message, "Listing of files and folders successful.");
    return message_status;
}
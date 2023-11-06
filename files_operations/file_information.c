#include "../headers.h"

acknowledgmentMessage additional_information_of_file(char file_name[MAX_LENGTH], acknowledgmentMessage message_status){
    printf("Additional information of the file.\n");

    char file_path[MAX_LENGTH];
    strcpy(file_path, file_name);

    struct stat file_stat;

    if (stat(file_path, &file_stat) == -1) {
             printf("Error getting file information");
             strcpy(message_status.status_message, "Error getting file information");
    }
    else{
            printf("File details fetched successfully.\n");
            strcpy(message_status.status_message, "File details fetched successfully.");
            printf("File Size: %ld bytes\n", file_stat.st_size);
            printf("File Permissions: %o\n", file_stat.st_mode & (S_IRWXU | S_IRWXG | S_IRWXO));
            printf("Last Access Time: %s", ctime(&file_stat.st_atime));
            printf("Last Modification Time: %s", ctime(&file_stat.st_mtime));
            printf("Last Status Change Time: %s", ctime(&file_stat.st_ctime));

            const char *file_extension = strrchr(file_path, '.');
            if (file_extension) {
                printf("File Extension: %s\n", file_extension);
            } else {
                printf("File Extension: Not found\n");
            }

            if (file_extension) {
                // Determine the file type based on the file extension
                if (strcmp(file_extension, ".txt") == 0) {
                    printf("File Type: Text File\n");
                } else if (strcmp(file_extension, ".jpg") == 0 || strcmp(file_extension, ".jpeg") == 0) {
                    printf("File Type: JPEG Image\n");
                } else {
                    printf("File Type: Unknown\n");
                }
            } else {
                printf("File Type: Unknown\n");
            }

            printf("File Owner: %d\n", file_stat.st_uid);
            printf("File Group: %d\n", file_stat.st_gid);
            printf("File Inode Number: %lu\n", file_stat.st_ino);
            printf("File Device ID: %lx\n", file_stat.st_dev);
    }
    return message_status;
}
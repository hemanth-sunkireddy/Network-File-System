#include "../headers.h" 

/*#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>*/

void deleting_the_folder(char folder_name[MAX_LENGTH]) {
    //if you give direct folder1/folder2/folder3 ->deletes folder3
    //if you give folder1 -> delets all its subdirecties and files
    char message[256];
    DIR *d = opendir(folder_name);
    struct dirent *entry;

    if (!d) {
        sprintf(message, "Error opening folder: %s\n", strerror(errno));
        printf("%s", message);
        // return 1;
    }

    while ((entry = readdir(d))) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        char path[MAX_LENGTH];
        snprintf(path, sizeof(path), "%s/%s", folder_name, entry->d_name);

        // if (entry->d_type == DT_DIR) {
        //     // Recursive call to delete subfolders
        //     if (deleteFolder(path) != 0) {
        //         closedir(d);
        //         // return 1;
        //     }
        // } else {
        //     if (remove(path) != 0) {
        //         sprintf(message, "Error deleting file: %s\n", strerror(errno));
        //         printf("%s", message);
        //         closedir(d);
        //         // return 1;
        //     }
        // }
    }

    closedir(d);

    if (rmdir(folder_name) != 0) {
        sprintf(message, "Error deleting folder: %s\n", strerror(errno));
        printf("%s", message);
        // return 1;
    }

    sprintf(message, "Folder \"%s\" and its contents deleted successfully.\n", folder_name);
    
    printf("%s", message);//or return message

    // return 0;
}

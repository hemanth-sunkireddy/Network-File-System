#include "../headers.h"

// /*
// #include <stdio.h>
// #include <stdlib.h>
// #include <sys/stat.h>
// #include <sys/types.h>
// #include <dirent.h>
// #include <string.h>
// #include <unistd.h>
// #include <time.h>
// */




// // Function to calculate the total size of a folder
// long long calculateTotalSize(const char *folderPath) {
//     struct stat st;
//     if (lstat(folderPath, &st) != 0) {
//         perror("Error getting folder information");
//         return 0;
//     }

//     if (!S_ISDIR(st.st_mode)) {
//         // It's a file
//         return st.st_size;
//     }

//     // It's a directory, so calculate the total size
//     long long totalSize = 0;
//     DIR *d = opendir(folderPath);
//     struct dirent *entry;
//     if (!d) {
//         perror("Error opening folder");
//         return 0;
//     }

//     while ((entry = readdir(d))) {
//         if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
//             continue;
//         }

//         char path[PATH_MAX];
//         snprintf(path, sizeof(path), "%s/%s", folderPath, entry->d_name);
//         totalSize += calculateTotalSize(path);
//     }

//     closedir(d);
//     return totalSize;
// }

// void countFolderContents(const char *folderPath, int *numFiles, int *numSubfolders) {
//     DIR *d = opendir(folderPath);

//     if (!d) {
//         perror("Error opening folder");
//         return;
//     }

//     struct dirent *entry;
//     *numFiles = 0;
//     *numSubfolders = 0;

//     while ((entry = readdir(d)) != NULL) {
//         if (entry->d_type == DT_DIR) {
//             if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
//                 (*numSubfolders)++;
//             }
//         } else {
//             (*numFiles)++;
//         }
//     }

//     closedir(d);
// }

void additional_information_of_folder(char folder_name[MAX_LENGTH]){ 
//     const char *folderPath = "folder_1/12";

//     // Folder information
//     struct stat st;
//     if (lstat(folderPath, &st) == 0) {
//         printf("Folder Information for %s:\n", folderPath);
//         printf("Size: %lld bytes\n", (long long)st.st_size);

//         // Permissions
//         printf("Permissions: ");
//         printf((S_ISDIR(st.st_mode)) ? "d" : "-");
//         printf((st.st_mode & S_IRUSR) ? "r" : "-");
//         printf((st.st_mode & S_IWUSR) ? "w" : "-");
//         printf((st.st_mode & S_IXUSR) ? "x" : "-");
//         printf((st.st_mode & S_IRGRP) ? "r" : "-");
//         printf((st.st_mode & S_IWGRP) ? "w" : "-");
//         printf((st.st_mode & S_IXGRP) ? "x" : "-");
//         printf((st.st_mode & S_IROTH) ? "r" : "-");
//         printf((st.st_mode & S_IWOTH) ? "w" : "-");
//         printf((st.st_mode & S_IXOTH) ? "x" : "-");
//         printf("\n");

//         // Print modified and created times
       
//        printf("Last Modified Time: %s", ctime(&st.st_mtime));
// 	printf("Created Time: %s", ctime(&st.st_ctime));


//         // Parent folder
//         char parentFolder[PATH_MAX];
//         if (realpath(folderPath, parentFolder) != NULL) {
//             char *lastSlash = strrchr(parentFolder, '/');
//             if (lastSlash != NULL) {
//                 *lastSlash = '\0';
//                 printf("Parent Folder: %s\n", parentFolder);
//             }
//         } else {
//             perror("Error getting parent folder information");
//         }
//     } else {
//         perror("Error getting folder information");
//         return 1;
//     }

//     // Calculate and print total size
//     long long totalSize = calculateTotalSize(folderPath);

//     if (totalSize > 0) {
//         // Convert bytes to kilobytes (KB)
//         double totalSizeKB = totalSize / 1024.0;
//         printf("Total Size of %s: %.2f KB\n", folderPath, totalSizeKB);
//     } else {
//         printf("Error calculating total size.\n");
//     }
    
//     int numFiles, numSubfolders;
//     countFolderContents(folderPath, &numFiles, &numSubfolders);

//     printf("Total Files: %d\n", numFiles);
//     printf("Total Subfolders: %d\n", numSubfolders);

//     return 0;
}

// /* subfolder/files in given folder, reccruesion not done
// sample output:
// Folder Information for folder_1:
// Size: 4096 bytes
// Permissions: drwxrwxr-x
// Last Modified Time: Sat Nov  4 14:51:36 2023
// Created Time: Sat Nov  4 14:51:36 2023
// Parent Folder: /home/popos/OSN
// Total Size of folder_1: 91.23 KB
// Total Files: 0
// Total Subfolders: 3

// */


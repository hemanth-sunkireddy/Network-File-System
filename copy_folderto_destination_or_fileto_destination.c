#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

#define MAX_PATH_LENGTH 512 // Increase buffer size

void copyFile(const char *sourcePath, const char *destinationPath) {
    FILE *sourceFile, *destinationFile;
    char buffer[1024];
    size_t bytesRead;

    sourceFile = fopen(sourcePath, "rb");
    if (sourceFile == NULL) {
        perror("Error opening source file");
        exit(EXIT_FAILURE);
    }

    destinationFile = fopen(destinationPath, "wb");
    if (destinationFile == NULL) {
        perror("Error opening destination file");
        fclose(sourceFile);
        exit(EXIT_FAILURE);
    }

    while ((bytesRead = fread(buffer, 1, sizeof(buffer), sourceFile)) > 0) {
        fwrite(buffer, 1, bytesRead, destinationFile);
    }

    fclose(sourceFile);
    fclose(destinationFile);

    printf("File copied successfully!\n");
}

void copyDirectory(const char *sourcePath, const char *destinationPath) {
    DIR *sourceDir;
    struct dirent *entry;

    sourceDir = opendir(sourcePath);
    if (sourceDir == NULL) {
        perror("Error opening source directory");
        exit(EXIT_FAILURE);
    }

    // Create the destination directory if it doesn't exist
    // Use platform-specific functions for creating directories
    #ifdef _WIN32
    _mkdir(destinationPath);
    #else
    mkdir(destinationPath, 0755);
    #endif

    while ((entry = readdir(sourceDir)) != NULL) {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            char sourceFilePath[MAX_PATH_LENGTH];
            char destinationFilePath[MAX_PATH_LENGTH];

            // Increase buffer size for snprintf
            snprintf(sourceFilePath, sizeof(sourceFilePath), "%s/%s", sourcePath, entry->d_name);
            snprintf(destinationFilePath, sizeof(destinationFilePath), "%s/%s", destinationPath, entry->d_name);

            if (entry->d_type == DT_REG) {
                copyFile(sourceFilePath, destinationFilePath);
            } else if (entry->d_type == DT_DIR) {
                copyDirectory(sourceFilePath, destinationFilePath);
            }
        }
    }

    closedir(sourceDir);
    printf("Directory copied successfully!\n");
}

int main() {
    
    // Use absolute paths for testing
    //1   Folder Paths
    char sourcePath[MAX_PATH_LENGTH] = "/home/popos/Cp"; // Change this to a valid source path
    char destinationPath[MAX_PATH_LENGTH] = "/home/popos/OSN/Concurrency"; // Change this to a valid destination path


     //2  FILE PATHS: ## mention file.txt in destination path
    char sourcePath[MAX_PATH_LENGTH] = "/home/popos/sql.txt"; // Change this to a valid source path
    char destinationPath[MAX_PATH_LENGTH] = "/home/popos/OSN/Concurrency/sql.txt"; // Change this to a valid destination path

    
    DIR *dir = opendir(sourcePath);
    if (dir) {
        closedir(dir);
        copyDirectory(sourcePath, destinationPath);
    } else {
        copyFile(sourcePath, destinationPath);
    }

    return 0;
}

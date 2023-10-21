#include <stdio.h>
#include <sys/stat.h>
#include <time.h>

int main() {
    const char *file_path = "example.txt"; // Provide the path to an existing file

    struct stat file_stat;

    if (stat(file_path, &file_stat) == -1) {
        perror("Error getting file information");
        return 1;
    }

    printf("File Size: %ld bytes\n", file_stat.st_size);
    printf("File Permissions: %o\n", file_stat.st_mode & (S_IRWXU | S_IRWXG | S_IRWXO));
    printf("Last Access Time: %s", ctime(&file_stat.st_atime));
    printf("Last Modification Time: %s", ctime(&file_stat.st_mtime));
    printf("Last Status Change Time: %s", ctime(&file_stat.st_ctime));

    return 0;
}

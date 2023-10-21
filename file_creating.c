#include <stdio.h>

int main() {
    const char *file_path = "empty_file.txt";

    // Create an empty file
    FILE *file = fopen(file_path, "w");
    if (file == NULL) {
        perror("Error creating an empty file");
        return 1;
    }

    fclose(file);
    printf("Empty file created at %s\n", file_path);

    return 0;
}
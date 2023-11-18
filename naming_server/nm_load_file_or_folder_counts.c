#include "headers.h"


void saveFileCounts(SS_Info ssx[MAX_STORAGE_SERVERS]) {
    FILE *file = fopen("file_counts.txt", "w");
    if (file == NULL) {
        perror("Error opening file");
        exit(1);
    }
    for (int i = 0; i < number_of_storage_servers; i++) {
        fprintf(file, "%d\n", ssx[i].num_of_files);
    }
    fclose(file);
}

void loadFileCounts(SS_Info ssx[MAX_STORAGE_SERVERS]) {
    FILE *file = fopen("file_counts.txt", "r");

    if (file == NULL) {
        perror("Error opening file");
        exit(1);
    }

    for (int i = 0; i < number_of_storage_servers; i++) {
        fscanf(file, "%d", &(ssx[i].num_of_files));
                printf("File count previously in %d server %d\n", i, ssx[i].num_of_files);

    }

    fclose(file);
}

void saveFolderCounts(SS_Info ssx[MAX_STORAGE_SERVERS]) {
    FILE *file = fopen("folder_counts.txt", "w");
    if (file == NULL) {
        perror("Error opening file");
        exit(1);
    }

    for (int i = 0; i < number_of_storage_servers; i++) {
        fprintf(file, "%d\n", ssx[i].num_of_folders);
    }

    fclose(file);
}

void loadFolderCounts(SS_Info ssx[MAX_STORAGE_SERVERS]) {
    FILE *file = fopen("folder_counts.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(1);
    }

    for (int i = 0; i < number_of_storage_servers; i++) {
        fscanf(file, "%d", &(ssx[i].num_of_folders));
    }

    fclose(file);
}
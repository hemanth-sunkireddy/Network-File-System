#include "headers.h"

void CreateNewStorageServer(SS_Info ssx[MAX_STORAGE_SERVERS])
{
    SS_Info* ssx_new=(SS_Info*)malloc(sizeof(SS_Info));
    int ss_socket;
    struct sockaddr_in ss_address;
    socklen_t ss_address_size;
    ssx[number_of_storage_servers-1]=(*ssx_new);
    ssx[number_of_storage_servers-1].SS_port=5000+number_of_storage_servers-1;
    ssx[number_of_storage_servers-1].storage_server_number=number_of_storage_servers-1;
    ssx[number_of_storage_servers-1].ss_socket=ss_socket;
    ssx[number_of_storage_servers-1].ss_address=ss_address;
    ssx[number_of_storage_servers-1].ss_address_size=ss_address_size;
    ssx[number_of_storage_servers-1].num_of_files=0;
    ssx[number_of_storage_servers-1].num_of_folders=0;
    free(ssx_new);
}


void FindNumberOfStorageServers(SS_Info ssx[MAX_STORAGE_SERVERS])
{
    loadFileCounts(ssx);
    loadFolderCounts(ssx);
    FILE *file = fopen("file_counts.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(1);
    }
    char line[MAX_PATH_LENGTH];
    int count=0;
    while(fgets(line, sizeof(line),file)){
        count++;
    }
    fclose(file);

    FILE *file2 = fopen("folder_counts.txt", "r");
    if (file2 == NULL) {
        perror("Error opening file");
        exit(1);
    }
    char line2[MAX_PATH_LENGTH];
    int count2=0;
    while(fgets(line2, sizeof(line2),file2)){
        count2++;
    }
    fclose(file2);

    number_of_storage_servers=(count>count2) ? count : count2;
}
#include "headers.h"


void ReadorWriteOperation(SS_Info ssx[MAX_STORAGE_SERVERS], acknowledgmentMessage message_status, fileNameAndOperation FilenameAndOperation, int ss_socket, Data_of_SS_SentToClient* data_of_ss_to_client)
{
    char path[MAX_PATH_LENGTH];
    strcpy(path,FilenameAndOperation.name_of_file_or_folder);
    printf("Path name: %s\n",path);
    
    for(int i=0;i<number_of_storage_servers;i++){
        for(int j=0;j<ssx[i].num_of_files+ssx[i].num_of_folders;j++){
            if((strncmp(path,ssx[i].paths_accessible[j].path,strlen(ssx[i].paths_accessible[j].path))==0 && path[strlen(ssx[i].paths_accessible[j].path)]=='\0') || (strncmp(path,ssx[i].paths_accessible[j].path,strlen(ssx[i].paths_accessible[j].path))==0 && path[strlen(ssx[i].paths_accessible[j].path)]=='/' && path[strlen(ssx[i].paths_accessible[j].path) + 1] != '\0')) {    
                data_of_ss_to_client->SS_port=ssx[0].SS_port;
                strcpy(data_of_ss_to_client->SS_ip,ssx[0].ip);
                data_of_ss_to_client->SS_no=ssx[0].storage_server_number;
                printf("Assigned the storage server: %d, ss port: %d.\n",data_of_ss_to_client->SS_no, data_of_ss_to_client->SS_port);
            }
        }
    }
}
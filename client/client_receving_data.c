#include "headers.h"

void client_receving_data(int client_socket,fileNameAndOperation file_or_folder_details){
    // acknowledgmentMessage ack_fromSS;
    if ( file_or_folder_details.operation_number == 1){
        acknowledgmentMessage message_status;
        char buffer[MAX_LENGTH];                
        int bytes_received = recv(client_socket, buffer, MAX_LENGTH, 0);

        // Null-terminate the received data.
        buffer[bytes_received] = '\0';

        printf("Received: %s\n", buffer);

        int length;
        sscanf(buffer, "%d", &length);

        printf("Length: %d\n", length);

        for (int i = 0; i < length; i++){
            bytes_received = recv(client_socket, buffer, MAX_LENGTH, 0);

            buffer[bytes_received] = '\0';

            printf("%s", buffer);
        }

        printf("Read response successfully.\n");
    }
    if ( file_or_folder_details.operation_number == 2){
        //writing
        char *input = NULL;
        size_t n = 50;
        //instead of fgets(fixed memory) ,use getline

        if(getline(&input, &n, stdin)==-1){//;reallocates if more memory required
            printf("Error in getline\n");
        }
        if(send(client_socket, input, n+1, 0)!=-1){
            printf("Data send successfully\n");
        }
        else{
            printf("Error in send \n");
        }
                

    }
    if ( file_or_folder_details.operation_number == 5){
        //filr info

    }
    if ( file_or_folder_details.operation_number == 6){
        //folder inf 
        folderInformation folderinfo;
        if(recv(client_socket,&folderinfo,sizeof(folderinfo),0)==-1){
            printf("error in getting folder data\n");
        }
        //print data
        
        printf("%s\n ",folderinfo.folder_name);
        printf("%d\n ",folderinfo.size);
        printf("%s\n ",folderinfo.Permissions);
        printf("%s\n ",folderinfo.last_modified_time);
        printf("%s\n ",folderinfo.created_time);
        printf("%s\n ",folderinfo.parent_folder);
        printf("%f\n ",folderinfo.total_size_of_folder_in_KB);
        printf("%d\n ",folderinfo.total_files);
        printf("%d\n ",folderinfo.total_folders);



    }
    if ( file_or_folder_details.operation_number == 7){
        //listing file or folder

    }






}

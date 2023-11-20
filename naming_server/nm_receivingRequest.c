#include "headers.h"


acknowledgmentMessage obtain_ss_info(SS_Info ssx[MAX_STORAGE_SERVERS],int ss_socket, acknowledgmentMessage message_status, fileNameAndOperation operation_and_fileorfolder_name, int* storage_server_connection_socket, Data_of_SS_SentToClient* data_of_ss_to_client,Copy_source_dest* copy_src_dest_oper){
    int operation_no=operation_and_fileorfolder_name.operation_number;
    if(operation_no==3 || operation_no==4 || operation_no==8 || operation_no==9){
        printf("OPERATION CREATE OR DELETE RECEIVED FROM CLIENT SUCCESSFULLY.\n");
        CreateandDeleteOperation(ssx,message_status,operation_and_fileorfolder_name, storage_server_connection_socket);
        printf("OPERATION RETURN STATUS CHECK.\n");
    }
    else if(operation_no==1 || operation_no==2 || operation_no==5 || operation_no==6 || operation_no==7){
        printf("Entered\n");
        ReadorWriteOperation(ssx,message_status,operation_and_fileorfolder_name, ss_socket, data_of_ss_to_client);
    }
    else if(operation_no==10 || operation_no==11){
          printf("Source path: %s\n", copy_src_dest_oper->source_path);
          printf("Destination Path: %s\n", copy_src_dest_oper->destination_path);
          CopyFileorFolderOperations(ssx,message_status,operation_and_fileorfolder_name, copy_src_dest_oper, storage_server_connection_socket);
    }
    return message_status;
}
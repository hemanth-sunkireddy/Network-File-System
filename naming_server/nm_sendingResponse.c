#include "headers.h"

void response_recieve_or_send(SS_Info ssx[MAX_STORAGE_SERVERS], int client_socket,struct acknowledgmentMessage message_status, int operation_number, int storage_server_connection_socket, Data_of_SS_SentToClient* data_of_ss_to_client){
    
    
    printf("The operation Number: %d\n", operation_number);
    printf("Just calling the sending response function.\n");
    char message_from_storage_server[MAX_LENGTH];
    char filename[50];
    strcpy(filename,"book_keeping.txt");

    // For the create and delete operation as well as copying copy file or folder operations also.
    if ( operation_number == 3 || operation_number == 4 || operation_number == 8 || operation_number == 9) { 
        int recieving_status = recv(storage_server_connection_socket, message_from_storage_server, MAX_LENGTH, 0);
        printf("Recieving status from storage server: %d\n", recieving_status);
        printf("MESSAGE FROM STORAGE SERVER: %s\n", message_from_storage_server);
        FILE *file = fopen(filename, "a");
        if (file == NULL) {
            perror("Error opening file");
            strcpy(message_status.status_message, "ERROR OPENING FILE");
            fprintf(file, "Operation number: %d\tMessage: %s\n",operation_number,message_status.status_message);
        }
        // Write the input string with newline character.
        fprintf(file, "Operation number: %d\tMessage: %s\n",operation_number,message_from_storage_server);
        // Close the file and free the input buffer.
        fclose(file);
    }
    

    // Now sending the recieved data of the storage server to the client. 
    if ( operation_number == 3 || operation_number == 4 || operation_number == 8 || operation_number == 9 ){
        send(client_socket, message_from_storage_server, MAX_LENGTH, 0);
        printf("MESSAGE TO THE CLIENT SENT SUCCESSFULLY.\n");
    }
    else{
        send(client_socket,data_of_ss_to_client, MAX_LENGTH, 0);
        printf("Storage server details/ status message sent to client successfully.\n");
        FILE *file = fopen(filename, "a");
        if (file == NULL) {
            perror("Error opening file");
            strcpy(message_status.status_message, "ERROR OPENING FILE");
        }
        // Write the input string with newline character.
        fprintf(file, "Operation number: %d\tCorresponding storage server number: %d\n",operation_number,data_of_ss_to_client->SS_no);
        // Close the file and free the input buffer.
        fclose(file);
    }
}
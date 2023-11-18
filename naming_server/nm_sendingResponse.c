#include "headers.h"

void response_recieve_or_send(SS_Info ssx[MAX_STORAGE_SERVERS], int client_socket,struct acknowledgmentMessage message_status, int operation_number, int storage_server_connection_socket){
    printf("The operation Number: %d\n", operation_number);
    printf("Just calling the sending response function.\n");
    char message_from_storage_server[MAX_LENGTH];
    int recieving_status = recv(storage_server_connection_socket, message_from_storage_server, MAX_LENGTH, 0);
    printf("Recieving status from storage server: %d\n", recieving_status);
    printf("MESSAGE FROM STORAGE SERVER: %s\n", message_from_storage_server);
    // printf("Status of the operation from storage server: %d\n", message_status.status_of_operation);

    // Now sending the recieved data of the storage server to the client. 
    if ( operation_number == 3 || operation_number == 4 || operation_number == 8 || operation_number == 9 ){
        send(client_socket, message_from_storage_server, MAX_LENGTH, 0);
        printf("MESSAGE TO THE CLIENT SENT SUCCESSFULLY.\n");
    }
}
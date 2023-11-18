#include "headers.h"

void response_recieve_or_send(SS_Info ssx[MAX_STORAGE_SERVERS], int client_socket,struct acknowledgmentMessage message_status, int operation_number){
    printf("The operation Number: %d\n", operation_number);
    printf("Just calling the sending response function.\n");
    // printf("Status of the operation from storage server: %d\n", message_status.status_of_operation);
}
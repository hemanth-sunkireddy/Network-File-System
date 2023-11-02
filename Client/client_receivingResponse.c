#include "../headers.h"

void receive_server_request(int client_socket)
{
    acknowledgmentMessage message_status;
    recv(client_socket, &message_status, MAX_LENGTH * MAX_LENGTH, 0);

    printf("%s\n", message_status.status_message);
    printf("%d\n", message_status.file_or_folder_content[0].total_chunks);
    for ( int i = 0; i < message_status.file_or_folder_content[0].total_chunks; i++ ) { 
        printf("%s", message_status.file_or_folder_content[i].data);
    }
    printf("Recieved server response.\n");
}
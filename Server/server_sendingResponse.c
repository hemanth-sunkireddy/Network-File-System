#include "../headers.h"

void send_server_request(acknowledgmentMessage message_status, int client_socket)
{
    // for ( int i = 0; i < message_status.file_or_folder_content[0].total_chunks; i++ ) { 
    //     printf("%s", message_status.file_or_folder_content[i].data);
    // }
    
    send(client_socket, &message_status, MAX_LENGTH * MAX_LENGTH, 0);

}
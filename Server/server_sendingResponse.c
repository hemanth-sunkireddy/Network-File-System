#include "../headers.h"

void send_server_request(acknowledgmentMessage message_status, int client_socket)
{
    char length_of_string[MAX_LENGTH];
    printf("%d\n", message_status.file_or_folder_content[0].total_chunks);
    sprintf(length_of_string, "%d", message_status.file_or_folder_content[0].total_chunks);
    
    // Send the length of the data as a separate message.
    send(client_socket, length_of_string, MAX_LENGTH, 0);
    printf("Length of the packet sent successfully\n");
    
    for (int i = 0; i < message_status.file_or_folder_content[0].total_chunks; i++) {
        
        send(client_socket, message_status.file_or_folder_content[i].data, MAX_LENGTH, 0);
    }
    
    printf("Sent response successfully\n");
}

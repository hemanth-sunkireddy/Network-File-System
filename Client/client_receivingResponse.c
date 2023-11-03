#include "../headers.h"

void receive_server_request(int client_socket)
{
    acknowledgmentMessage message_status;
    char buffer[MAX_LENGTH];
    
    int bytes_received = recv(client_socket, buffer, MAX_LENGTH, 0);

    // Null-terminate the received data.
    buffer[bytes_received] = '\0';

    printf("Received: %s\n", buffer);

    int length;
    sscanf(buffer, "%d", &length);

    printf("Length: %d\n", length);

    for (int i = 0; i < length; i++) {
        bytes_received = recv(client_socket, buffer, MAX_LENGTH, 0);

        buffer[bytes_received] = '\0';

        printf("%s", buffer);
    }

    printf("Read response successfully.\n");
}

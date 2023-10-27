#include "../headers.h"

void receive_server_request(int client_socket)
{
    char buffer[MAX_LENGTH];
    bzero(buffer, MAX_LENGTH);
    recv(client_socket, buffer, sizeof(buffer), 0);
    printf("Server: %s\n", buffer);
    printf("Recieved server request.\n");
}
#include "headers.h"

void receive_client_request(char* buffer, int client_socket)
{
    bzero(buffer, MAX_LENGTH);
    recv(client_socket, buffer, MAX_LENGTH, 0);
    printf("Client: %s\n", buffer);
}
#include "headers.h"

void receive_server_request(char* buffer, int sock)
{
    bzero(buffer, MAX_LENGTH);
    recv(sock, buffer, sizeof(buffer), 0);
    printf("Server: %s\n", buffer);
}
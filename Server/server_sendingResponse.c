#include "../headers.h"

void send_server_request(char* buffer, int client_socket)
{
    bzero(buffer, 1024);
    fgets(buffer, MAX_LENGTH, stdin);
    int length_of_buffer = strlen(buffer); 
    buffer [ length_of_buffer - 1 ] = '\0'; 
    printf("Serverhere: %s\n", buffer);
    send(client_socket, buffer, strlen(buffer), 0);
}
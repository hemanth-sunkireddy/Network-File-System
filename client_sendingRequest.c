#include "headers.h"

void send_client_request(char* buffer, int sock){
        bzero(buffer, MAX_LENGTH);
        printf("Enter your request and wait till the server sends response back:");
        fgets(buffer, MAX_LENGTH, stdin);
        int length_of_buffer = strlen(buffer);
        buffer [ length_of_buffer - 1 ] = '\0'; 
        printf("Client: %s\n", buffer);
        send(sock, buffer, strlen(buffer), 0);
}

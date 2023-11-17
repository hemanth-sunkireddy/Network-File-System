#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int client_socket;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    // Create socket
    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Initialize server address struct
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    // Connect to server
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    printf("Connected to the server.\n");

    // Send data to the server continuously
    while (1) {
        char message[] = "Hello, Server!";
        if (send(client_socket, message, strlen(message), 0) == -1) {
            perror("Send failed");
            exit(EXIT_FAILURE);
        }
        else{

        printf("Data sent to server: %s\n", message);
        }
        char status_message[1024];
        if ( recv(client_socket, status_message, 1024, 0) > 0){
            printf("Data recieved successfully: %s\n", message);
        }
        sleep(2); // Sleep for 2 seconds before sending the next message
    }

    // Close the socket (This line won't be reached in the current example)
    close(client_socket);

    return 0;
}

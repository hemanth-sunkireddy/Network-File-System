#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024

int main() {
    int server_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);

    // Create socket
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Initialize server address struct
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Bind the socket to the specified IP and port
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Binding failed");
        exit(EXIT_FAILURE);
    } else {
        printf("Binded successfully.\n");
    }

    int reuse = 1;
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) == -1) {
        perror("Setting SO_REUSEADDR failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_socket, 5) == -1) {
        perror("Listening failed");
        exit(EXIT_FAILURE);
    } else {
        printf("Listening.\n");
    }

    printf("Server listening on port 8080...\n");
    printf("Server IP address: %s\n", inet_ntoa(server_addr.sin_addr));

    // Initialize client_sockets array
    int client_sockets[BUFFER_SIZE];
    for (int i = 0; i < BUFFER_SIZE; ++i) {
        client_sockets[i] = 0;
    }

    // FD SET INITIALISATION.
    fd_set read_fds;

    while (1) {
        FD_ZERO(&read_fds);
        FD_SET(server_socket, &read_fds);

        int max_sd = server_socket;

        for (int i = 0; i < BUFFER_SIZE; ++i) {
            int client_socket = client_sockets[i];

            if (client_socket > 0) {
                FD_SET(client_socket, &read_fds);
                if (client_socket > max_sd) {
                    max_sd = client_socket;
                }
            }
        }

        // Wait for activity on any of the sockets
        if (select(max_sd + 1, &read_fds, NULL, NULL, NULL) == -1) {
            perror("Select failed");
            exit(EXIT_FAILURE);
        }

        // Check for incoming connection
        if (FD_ISSET(server_socket, &read_fds)) {
            int new_socket;

            if ((new_socket = accept(server_socket, (struct sockaddr *)&client_addr, &addr_len)) == -1) {
                perror("NEW CONNECTION ACCEPTANCE FAILED.\n");
            } else {
                printf("NEW CONNECTION ACCEPTANCE SUCCESSFUL.\n");
                printf("New connection, socket fd is %d, IP is: %s, port: %d\n",
                       new_socket, inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

                // Add new socket to array of client sockets
                for (int i = 0; i < BUFFER_SIZE; ++i) {
                    if (client_sockets[i] == 0) {
                        client_sockets[i] = new_socket;
                        break;
                    }
                }
            }
        }

        // Check for data from clients
        for (int i = 0; i < BUFFER_SIZE; ++i) {
            int client_socket = client_sockets[i];

            if (FD_ISSET(client_socket, &read_fds)) {
                char buffer[BUFFER_SIZE];
                int read_size = recv(client_socket, buffer, sizeof(buffer), 0);

                if (read_size > 0) {
                    printf("Received from client at socket %d successful.\n", client_socket);
                    printf("Request from client at socket %d: %s\n", client_socket, buffer);
                } else if (read_size == 0) {
                    // Connection closed by the client
                    printf("Client at socket %d disconnected\n", client_socket);
                    close(client_socket);
                    client_sockets[i] = 0;  // Reset the client socket in the array
                } else {
                    perror("Receive failed");
                    close(client_socket);
                    client_sockets[i] = 0;  // Reset the client socket in the array
                }

                int send_status = send(client_socket, buffer, sizeof(buffer), 0);
                if ( send_status > 0){
                    printf("Status sent succesfully to client.\n");
                }
            }
        }

        // Print the list of connected clients
        printf("Connected clients: ");
        for (int i = 0; i < BUFFER_SIZE; ++i) {
            int client_socket = client_sockets[i];
            if (client_socket > 0) {
                printf("%d ", client_socket);
            }
        }
        printf("\n");
    }

    return 0;
}

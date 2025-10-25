#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define NS_PORT 6000       // Naming Server port
#define SS_PORT 8080       // Storage Server port (from naming server)
#define BUFFER_SIZE 4096

void connect_to_storage_server(const char *ip, int port, const char *command);

int main() {
    int client_socket;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    char command[BUFFER_SIZE];

    // Connect to the Naming Server
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(NS_PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection to Naming Server failed");
        close(client_socket);
        exit(1);
    }

    printf("Connected to Naming Server at 127.0.0.1:%d\n", NS_PORT);
    printf("Enter operation (UPLOAD filename | DOWNLOAD filename | DELETE filename | LIST):\n> ");
    fgets(command, sizeof(command), stdin);
    command[strcspn(command, "\n")] = 0;  // remove newline

    // Send command to naming server
    send(client_socket, command, strlen(command), 0);

    // Receive storage server details
    char ss_info[BUFFER_SIZE];
    int bytes = recv(client_socket, ss_info, sizeof(ss_info) - 1, 0);
    if (bytes <= 0) {
        printf("Failed to receive storage server info.\n");
        close(client_socket);
        return 1;
    }
    ss_info[bytes] = '\0';
    close(client_socket);

    printf("Naming Server response: %s\n", ss_info);

    // Extract IP and Port
    char ss_ip[64];
    int ss_port;
    sscanf(ss_info, "%s %d", ss_ip, &ss_port);

    // Connect to Storage Server and execute command
    connect_to_storage_server(ss_ip, ss_port, command);

    return 0;
}

void connect_to_storage_server(const char *ip, int port, const char *command) {
    int ss_socket;
    struct sockaddr_in ss_addr;
    char buffer[BUFFER_SIZE];

    ss_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (ss_socket < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    ss_addr.sin_family = AF_INET;
    ss_addr.sin_port = htons(port);
    ss_addr.sin_addr.s_addr = inet_addr(ip);

    if (connect(ss_socket, (struct sockaddr*)&ss_addr, sizeof(ss_addr)) < 0) {
        perror("Connection to Storage Server failed");
        close(ss_socket);
        return;
    }

    printf("Connected to Storage Server at %s:%d\n", ip, port);

    // Send operation command
    send(ss_socket, command, strlen(command), 0);

    // Handle UPLOAD operation
    if (strncmp(command, "UPLOAD", 6) == 0) {
        char *filename = strchr(command, ' ');
        if (!filename) {
            printf("No filename provided.\n");
            return;
        }
        filename++;

        FILE *fp = fopen(filename, "rb");
        if (!fp) {
            perror("Failed to open file");
            close(ss_socket);
            return;
        }

        int bytes;
        while ((bytes = fread(buffer, 1, BUFFER_SIZE, fp)) > 0) {
            send(ss_socket, buffer, bytes, 0);
        }
        send(ss_socket, "EOF", 3, 0);
        fclose(fp);
        printf("File uploaded successfully.\n");
    }

    // Handle DOWNLOAD operation
    else if (strncmp(command, "DOWNLOAD", 8) == 0) {
        char *filename = strchr(command, ' ');
        if (!filename) {
            printf("No filename provided.\n");
            return;
        }
        filename++;

        FILE *fp = fopen(filename, "wb");
        if (!fp) {
            perror("Failed to open file for writing");
            close(ss_socket);
            return;
        }

        int bytes;
        while ((bytes = recv(ss_socket, buffer, BUFFER_SIZE, 0)) > 0) {
            if (strncmp(buffer, "EOF", 3) == 0) break;
            fwrite(buffer, 1, bytes, fp);
        }
        fclose(fp);
        printf("File downloaded successfully.\n");
    }

    // Handle LIST or DELETE responses
    else {
        int bytes = recv(ss_socket, buffer, sizeof(buffer) - 1, 0);
        if (bytes > 0) {
            buffer[bytes] = '\0';
            printf("Server response:\n%s\n", buffer);
        }
    }

    close(ss_socket);
}

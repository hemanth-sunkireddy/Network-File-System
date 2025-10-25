#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <dirent.h>

#define PORT 8080
#define BUFFER_SIZE 4096
#define STORAGE_DIR "storage_data"

char *ip = "127.0.0.1";

// Function prototypes
void handle_client(int client_socket);
void upload_file(int client_socket, const char *filename);
void download_file(int client_socket, const char *filename);
void delete_file(int client_socket, const char *filename);
void list_files(int client_socket);

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size;
    char buffer[BUFFER_SIZE];

    // Ensure the storage directory exists
    mkdir(STORAGE_DIR, 0777);

    // Create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    // Configure address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(ip);

    // Bind socket
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(server_socket);
        exit(1);
    }

    // Listen for connections
    if (listen(server_socket, 5) == -1) {
        perror("Listen failed");
        close(server_socket);
        exit(1);
    }

    printf("Storage Server running at %s:%d\n", ip, PORT);

    // Accept clients continuously
    while (1) {
        addr_size = sizeof(client_addr);
        client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &addr_size);

        if (client_socket < 0) {
            perror("Accept failed");
            continue;
        }

        printf("Client connected!\n");

        handle_client(client_socket);

        close(client_socket);
        printf("Client disconnected.\n\n");
    }

    close(server_socket);
    return 0;
}

// Handle client requests
void handle_client(int client_socket) {
    char command[BUFFER_SIZE];
    int bytes_received = recv(client_socket, command, sizeof(command), 0);
    if (bytes_received <= 0) return;

    command[bytes_received] = '\0';

    // Parse operation
    char operation[16], filename[256];
    sscanf(command, "%s %s", operation, filename);

    printf("Received command: %s %s\n", operation, filename);

    if (strcmp(operation, "UPLOAD") == 0) {
        upload_file(client_socket, filename);
    } else if (strcmp(operation, "DOWNLOAD") == 0) {
        download_file(client_socket, filename);
    } else if (strcmp(operation, "DELETE") == 0) {
        delete_file(client_socket, filename);
    } else if (strcmp(operation, "LIST") == 0) {
        list_files(client_socket);
    } else {
        char msg[] = "Unknown operation\n";
        send(client_socket, msg, strlen(msg), 0);
    }
}

// === File Operation Functions ===

void upload_file(int client_socket, const char *filename) {
    char path[512];
    snprintf(path, sizeof(path), "%s/%s", STORAGE_DIR, filename);

    FILE *fp = fopen(path, "wb");
    if (!fp) {
        char msg[] = "Failed to open file for writing\n";
        send(client_socket, msg, strlen(msg), 0);
        return;
    }

    char buffer[BUFFER_SIZE];
    int bytes;
    while ((bytes = recv(client_socket, buffer, BUFFER_SIZE, 0)) > 0) {
        if (strncmp(buffer, "EOF", 3) == 0) break;
        fwrite(buffer, 1, bytes, fp);
    }

    fclose(fp);
    char msg[] = "Upload complete\n";
    send(client_socket, msg, strlen(msg), 0);
}

void download_file(int client_socket, const char *filename) {
    char path[512];
    snprintf(path, sizeof(path), "%s/%s", STORAGE_DIR, filename);

    FILE *fp = fopen(path, "rb");
    if (!fp) {
        char msg[] = "File not found\n";
        send(client_socket, msg, strlen(msg), 0);
        return;
    }

    char buffer[BUFFER_SIZE];
    int bytes;
    while ((bytes = fread(buffer, 1, BUFFER_SIZE, fp)) > 0) {
        send(client_socket, buffer, bytes, 0);
    }
    send(client_socket, "EOF", 3, 0);
    fclose(fp);
}

void delete_file(int client_socket, const char *filename) {
    char path[512];
    snprintf(path, sizeof(path), "%s/%s", STORAGE_DIR, filename);

    if (remove(path) == 0) {
        char msg[] = "File deleted successfully\n";
        send(client_socket, msg, strlen(msg), 0);
    } else {
        char msg[] = "File deletion failed\n";
        send(client_socket, msg, strlen(msg), 0);
    }
}

void list_files(int client_socket) {
    DIR *dir = opendir(STORAGE_DIR);
    if (!dir) {
        char msg[] = "Could not open storage directory\n";
        send(client_socket, msg, strlen(msg), 0);
        return;
    }

    struct dirent *entry;
    char buffer[BUFFER_SIZE] = "";
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] != '.')
            strcat(buffer, entry->d_name), strcat(buffer, "\n");
    }
    closedir(dir);

    send(client_socket, buffer, strlen(buffer), 0);
}

#include "headers.h"

char *ip = "127.0.0.1";

void create_socket(list_of_all_storage_servers *database) {
    for (int i = 0; i < CURRENT_STORAGE_SERVERS_COUNT; i++) {
        database->individual_storage_server[i].storage_server_number = i + 1;
        database->individual_storage_server[i].server_socket = socket(AF_INET, SOCK_STREAM, 0);
        if (database->individual_storage_server[i].server_socket < 0) {
            printf("Error in creating %d error.\n", i);
        } else {
            printf("TCP SERVER SOCKET FOR %d STORAGE SERVER CREATED.\n", i);
        }
    }
}

void bind_socket(list_of_all_storage_servers *database) {
    int bind_check;

    for (int i = 0; i < CURRENT_STORAGE_SERVERS_COUNT; i++) {
        memset(&database->individual_storage_server[i].server_address, '\0', sizeof(database->individual_storage_server[i].server_address));
        database->individual_storage_server[i].server_address.sin_family = AF_INET;
        database->individual_storage_server[i].server_address.sin_port = FIRST_STORAGE_SERVER_PORT + i;
        database->individual_storage_server[i].server_address.sin_addr.s_addr = inet_addr(ip);

        bind_check = bind(database->individual_storage_server[i].server_socket,
                          (struct sockaddr *)&database->individual_storage_server[i].server_address,
                          sizeof(database->individual_storage_server[i].server_address));
        if (bind_check < 0) {
            printf("Binding error for %d server, address already in use\n", i);
        } else {
            printf("%d server Binded to port number %d successfully.\n", i, FIRST_STORAGE_SERVER_PORT + i);
        }
    }
}

void start_listening(list_of_all_storage_servers *database) {
    int listen_check;

    for (int i = 0; i < CURRENT_STORAGE_SERVERS_COUNT; i++) {
        listen_check = listen(database->individual_storage_server[i].server_socket, MAX_CLIENTS_FOR_SERVER);
        if (listen_check < 0) {
            printf("Listen error for %d server\n", i);
        } else {
            printf("Listening on %d server...\n", i);
        }
    }
}

int main() {
    socklen_t address_size;

    list_of_all_storage_servers huge_database;

    create_socket(&huge_database);
    printf("\n\n");
    bind_socket(&huge_database);
    printf("\n\n");
    start_listening(&huge_database);
    printf("\n\n");

    while (1) {
        printf("Wait till the client connects:\n");

        // Accept the client_socket
        for (int i = 0; i < CURRENT_STORAGE_SERVERS_COUNT; i++) {
            address_size = sizeof(huge_database.individual_storage_server[i].client_address);
            huge_database.individual_storage_server[i].client_socket =
                accept(huge_database.individual_storage_server[i].server_socket,
                       (struct sockaddr *)&huge_database.individual_storage_server[i].client_address, &address_size);
            printf("Client connected to %d storage server.\n", i);
            printf("Wait till the client sends to the request:\n");
        }
    }

    return 0;
}

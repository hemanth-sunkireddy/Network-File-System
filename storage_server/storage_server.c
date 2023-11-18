#include "headers.h"



void create_socket(list_of_all_storage_servers *database, int index_of_storage_server) {
        database->individual_storage_server[index_of_storage_server].storage_server_number = index_of_storage_server + 1;
        database->individual_storage_server[index_of_storage_server].server_socket = socket(AF_INET, SOCK_STREAM, 0);
        if (database->individual_storage_server[index_of_storage_server].server_socket < 0) {
            printf("Error in creating %d error.\n", index_of_storage_server);
        } else {
            printf("TCP SERVER SOCKET FOR %d STORAGE SERVER CREATED.\n", index_of_storage_server);
        }
}

void bind_socket(list_of_all_storage_servers *database, int index_of_storage_server) {
    int bind_check;
        memset(&database->individual_storage_server[index_of_storage_server].server_address, '\0', sizeof(database->individual_storage_server[index_of_storage_server].server_address));
        database->individual_storage_server[index_of_storage_server].server_address.sin_family = AF_INET;
        database->individual_storage_server[index_of_storage_server].server_address.sin_port = FIRST_STORAGE_SERVER_PORT + index_of_storage_server;
        database->individual_storage_server[index_of_storage_server].server_address.sin_addr.s_addr = inet_addr(ip);

        bind_check = bind(database->individual_storage_server[index_of_storage_server].server_socket,
                          (struct sockaddr *)&database->individual_storage_server[index_of_storage_server].server_address,
                          sizeof(database->individual_storage_server[index_of_storage_server].server_address));
        if (bind_check < 0) {
            printf("Binding error for %d server, address already in use\n", index_of_storage_server);
        } else {
            printf("%d server Binded to port number %d successfully.\n", index_of_storage_server, (FIRST_STORAGE_SERVER_PORT + index_of_storage_server));
        }
}

void start_listening(list_of_all_storage_servers *database, int index_of_storage_server) {
    int listen_check;
        listen_check = listen(database->individual_storage_server[index_of_storage_server].server_socket, MAX_CLIENTS_FOR_SERVER);
        if (listen_check < 0) {
            printf("Listen error for %d server\n", index_of_storage_server);
        } else {
            printf("Listening on %d server...\n", index_of_storage_server);
        }
}
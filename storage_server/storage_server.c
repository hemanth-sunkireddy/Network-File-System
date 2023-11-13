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


void main_storage_server_initialisation(){
    // Initialisation of main storage server need to be implement. (Implementing)
}

int main() {

    main_storage_server_initialisation();
    
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
        for (int index_of_storage_server = 0; index_of_storage_server < CURRENT_STORAGE_SERVERS_COUNT; index_of_storage_server++) {
            address_size = sizeof(huge_database.individual_storage_server[index_of_storage_server].client_address);
            huge_database.individual_storage_server[index_of_storage_server].client_socket =accept(huge_database.individual_storage_server[index_of_storage_server].server_socket, (struct sockaddr *)&huge_database.individual_storage_server[index_of_storage_server].client_address, &address_size);
            printf("namingServer/Client connected to %d storage server.\n", index_of_storage_server);
            printf("Wait till the namingServer/Client sends to the request:\n");

            
            acknowledgmentMessage message_status;
            fileNameAndOperation details_of_operation;

            // Recieve the request
             recv(huge_database.individual_storage_server[index_of_storage_server].client_socket, &details_of_operation, MAX_LENGTH, 0);
            printf("client/namingServer Asked for this file / folder: %s\n", details_of_operation.name_of_file_or_folder);

            int operation_number = details_of_operation.operation_number; 

            printf("The operation number he had choosen.:%d\n", operation_number);

            // message_status =  receive_client_request(huge_database.individual_storage_server[index_of_storage_server].client_socket, message_status, index_of_storage_server);
            // printf("OPERATION STATUS: %s\n", message_status.status_message);

            // Send the Response 
            send_server_request(message_status, huge_database.individual_storage_server[index_of_storage_server].client_socket);

            // CLose the socket
            close(huge_database.individual_storage_server[index_of_storage_server].client_socket);
            printf("Client disconnected from storage server %d.\n\n", huge_database.individual_storage_server[index_of_storage_server].storage_server_number);
        }
    }

    return 0;
}

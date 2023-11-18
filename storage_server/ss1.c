#include "headers.h"

char *ip = "127.0.0.1";




int main() {
    int index_of_storage_server = 0;
    int current_storage_servers = 4; 
    int* pointer_to_current_storage_servers = &current_storage_servers;
    // main_storage_server_initialisation();
    
    socklen_t address_size;

    list_of_all_storage_servers huge_database;

    create_socket(&huge_database, index_of_storage_server);
    bind_socket(&huge_database, index_of_storage_server);
    start_listening(&huge_database, index_of_storage_server);

    while (1) {
        printf("Wait till the client connects:\n");
            // Accept the client_socket
            address_size = sizeof(huge_database.individual_storage_server[index_of_storage_server].client_address);
            huge_database.individual_storage_server[index_of_storage_server].client_socket =accept(huge_database.individual_storage_server[index_of_storage_server].server_socket, (struct sockaddr *)&huge_database.individual_storage_server[index_of_storage_server].client_address, &address_size);
            printf("namingServer/Client connected to %d storage server.\n", index_of_storage_server);
            printf("Wait till the namingServer/Client sends to the request:\n");

            
            acknowledgmentMessage message_status;
            fileNameAndOperation details_of_operation;

            // Recieve the request
            message_status =  receive_client_request(huge_database.individual_storage_server[index_of_storage_server].client_socket, message_status, index_of_storage_server, pointer_to_current_storage_servers);
            printf("OPERATION STATUS: %s\n", message_status.status_message);
            printf("Current storage servers after addding one new storage server.%d\n", current_storage_servers);
            // Send the Response 
            send_server_request(message_status, huge_database.individual_storage_server[index_of_storage_server].client_socket);

            // CLose the socket
            close(huge_database.individual_storage_server[index_of_storage_server].client_socket);
            printf("Client disconnected from storage server %d.\n\n", huge_database.individual_storage_server[index_of_storage_server].storage_server_number);
    }

    return 0;
}





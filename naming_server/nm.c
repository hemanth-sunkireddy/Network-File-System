#include "headers.h"

char *ip = "127.0.0.1";
int port = PORT;
char message[1024];
int main(){

  SS_Info ssx[MAX_STORAGE_SERVERS];
  for (int i = 0; i < MAX_STORAGE_SERVERS; i++){
    ssx[i].SS_port = PORT + i;
    strcpy(ssx[i].ip,ip);
    ssx[i].storage_server_number=i;
    printf("Storage server %d number connected to port number %d\n", i, PORT + i);
  }
  int nm_server_socket, ss_socket;
  struct sockaddr_in nm_server_address, client_address;
  socklen_t address_size;
  char buffer[MAX_LENGTH];
  
  // Activate socket for the nm_server.
  nm_server_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (nm_server_socket < 0){
    perror("Socket error");
    exit(1);
  }
  else{
    printf("TCP nm_server socket created.\n");
  }

  memset(&nm_server_address, '\0', sizeof(nm_server_address));
  nm_server_address.sin_family = AF_INET;
  nm_server_address.sin_port = NAMING_SERVER_MAIN_PORT;
  nm_server_address.sin_addr.s_addr = inet_addr(ip);
  
  // Bind the server socket.
  int bind_check;
  bind_check = bind(nm_server_socket, (struct sockaddr*)&nm_server_address, sizeof(nm_server_address));
  if (bind_check < 0){
    perror("Bind error");
    exit(1);
  }
  else{
    printf("Binded to the port number: %d\n", NAMING_SERVER_MAIN_PORT);
  }

  // Listen server
  listen(nm_server_socket, 20);
  printf("\n\nListening...\n");
  
  // Code for multiple clients. 
  // Initialize client_sockets array
  int max_clients = 20;
    int client_sockets[max_clients];
    for (int i = 0; i < max_clients; ++i) {
        client_sockets[i] = 0;
    }

    // FD SET INITIALISATION.
    fd_set read_fds;

  while(1){
        FD_ZERO(&read_fds);
        FD_SET(nm_server_socket, &read_fds);

        int max_sd = nm_server_socket;

        for (int i = 0; i < max_clients; ++i) {
            int client_socket = client_sockets[i];

            if (client_socket > 0) {
                FD_SET(client_socket, &read_fds);
                if (client_socket > max_sd) {
                    max_sd = client_socket;
                }
            }
        }

        

        printf("Wait till the client connects:\n");


        // Wait for activity on any of the sockets
        if (select(max_sd + 1, &read_fds, NULL, NULL, NULL) == -1) {
            perror("Select failed");
            exit(EXIT_FAILURE);
        }

        // Check for incoming connection
        if (FD_ISSET(nm_server_socket, &read_fds)) {
            int new_socket;

            if ((new_socket = accept(nm_server_socket, (struct sockaddr *)&client_address, &address_size)) == -1) {
                perror("NEW CONNECTION ACCEPTANCE FAILED.\n");
            } else {
                printf("NEW CONNECTION ACCEPTANCE SUCCESSFUL.\n");
                printf("New connection, socket fd is %d, IP is: %s, port: %d\n",
                       new_socket, inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));

                // Add new socket to array of client sockets
                for (int i = 0; i < max_clients; ++i) {
                    if (client_sockets[i] == 0) {
                        client_sockets[i] = new_socket;
                        break;
                    }
                }
            }
        }


        acknowledgmentMessage message_status;
        fileNameAndOperation client_operation_number_path_name;

        // Check for data from clients
        for (int i = 0; i < max_clients; ++i) {
            int client_socket = client_sockets[i];

            if (client_socket > 0 && FD_ISSET(client_socket, &read_fds)) {
                int recieve_status = recv(client_socket, &client_operation_number_path_name, MAX_LENGTH, 0);
                printf("Status of the recieving:%d\n", recieve_status);
                printf("Operation number client asked:%d\n", client_operation_number_path_name.operation_number);
                printf("PATH CLIENT ASKED: %s\n", client_operation_number_path_name.name_of_file_or_folder);

                if (recieve_status > 0) {
                    printf("Received from client at socket %d successful.\n", client_socket);
                } else if (recieve_status == 0) {
                    // Connection closed by the client
                    printf("Client at socket %d disconnected\n", client_socket);
                    close(client_socket);
                    client_sockets[i] = 0; // Reset the client socket in the array
                } else {
                    perror("Receive failed");
                    close(client_socket);
                    client_sockets[i] = 0; // Reset the client socket in the array
                }

                // Recieve the request from the client. 
                int storage_server_connection_socket; 
                Data_of_SS_SentToClient data_of_ss_to_client;


                // Defining the struct of the operation 10 and 11. 
                Copy_source_dest copy_src_dest_oper;
                strcpy(copy_src_dest_oper.source_path, client_operation_number_path_name.name_of_file_or_folder);
                copy_src_dest_oper.operation_number = client_operation_number_path_name.operation_number;
                if (client_operation_number_path_name.operation_number == 10 || client_operation_number_path_name.operation_number == 11 ) { 
                  char destinaion_path[MAX_LENGTH];
                  int recieving_status_2 = recv(ss_socket, destinaion_path, MAX_LENGTH, 0);
                  printf("Recieving status of the destination path: %d\n", recieving_status_2);
                  printf("Destination path: %s\n",destinaion_path);
                  strcpy(copy_src_dest_oper.destination_path, destinaion_path);
                }

                
                message_status = obtain_ss_info(ssx,client_socket, message_status,client_operation_number_path_name, &storage_server_connection_socket, &data_of_ss_to_client, &copy_src_dest_oper);

                // Recieve the response from the storage server. 
                response_recieve_or_send(ssx, client_socket, message_status, client_operation_number_path_name.operation_number, storage_server_connection_socket, &data_of_ss_to_client);
                if(client_operation_number_path_name.operation_number==10 || client_operation_number_path_name.operation_number==11){
                  close(ss_socket);
                }
            }
        }

        // Print the list of connected clients
        printf("Connected clients: ");
        for (int i = 0; i < max_clients; ++i) {
            int client_socket = client_sockets[i];
            if (client_socket > 0) {
                printf("%d ", client_socket);
            }
        }
        printf("\n");

  }

  // Close the socket
  close(ss_socket);
  printf("Client disconnected.\n");


  return 0;
}

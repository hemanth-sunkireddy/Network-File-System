#include "headers.h"

void recieve_request(char* buffer, int client_socket){
    bzero(buffer, MAX_LENGTH);
    recv(client_socket, buffer, MAX_LENGTH, 0);
    printf("Client: %s\n", buffer);
}


void send_request(char* buffer, int client_socket){
    bzero(buffer, 1024);
    fgets(buffer, MAX_LENGTH, stdin);
    int length_of_buffer = strlen(buffer); 
    buffer [ length_of_buffer - 1 ] = '\0'; 
    printf("Server: %s\n", buffer);
    send(client_socket, buffer, strlen(buffer), 0);
}

int main(){

  int server_socket, client_socket;
  struct sockaddr_in server_address, client_address;
  socklen_t address_size;
  char buffer[MAX_LENGTH];
  
  // Activate socket for the server.
  server_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (server_socket < 0){
    perror("Socket error");
    exit(1);
  }
  else{
    printf("TCP server socket created.\n");
  }

  memset(&server_address, '\0', sizeof(server_address));
  server_address.sin_family = AF_INET;
  server_address.sin_port = port;
  server_address.sin_addr.s_addr = inet_addr(ip);
  
  // Bind the server socket.
  int bind_check;
  bind_check = bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address));
  if (bind_check < 0){
    perror("Bind error");
    exit(1);
  }
  else{
    printf("Binded to the port number: %d\n", port);
  }

  

  while(1){

        // Listen server
        listen(server_socket, 5);
        printf("Listening...\n");

        printf("Wait till the client sends the request:\n");

        // Accept the client_socket 
        address_size = sizeof(client_address);
        client_socket = accept(server_socket, (struct sockaddr*)&client_address, &address_size);
        printf("Client connected.\n");

        // Recieve the request
        recieve_request(buffer, client_socket);
        
      
        // Send the response 
        printf("Enter the response for sending to the client:");
        send_request(buffer, client_socket);

        // CLose the socket
        close(client_socket);
        printf("Client disconnected.\n\n");
  }


  return 0;
}

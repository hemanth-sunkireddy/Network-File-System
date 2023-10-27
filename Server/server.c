#include "../headers.h"

char *ip = "127.0.0.1";
int port = PORT;
char message[1024];
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

       FILE *file_pointer= fopen("README.md", "r");

            // Check if the file opened successfully.
            if (file_pointer == NULL) {
                printf("Could not open file.\n");
                // strcpy ( message_status.status_message, "COULD NOT OPEN FILE");
                return ;
                
            }
            else{
                printf("File opened successfully\n");
                // strcpy ( message_status.status_message, "OPENED SUCCESSFULLY");
                return ;
                
            }

        // Listen server
        listen(server_socket, 5);
        printf("Listening...\n");

        printf("Wait till the client connects:\n");

        // Accept the client_socket 
        address_size = sizeof(client_address);
        client_socket = accept(server_socket, (struct sockaddr*)&client_address, &address_size);
        printf("Client connected.\n");
        printf("Wait till the client sends the request:\n");

        acknowledgmentMessage message_status;


         

        // Recieve the request
        message_status =  receive_client_request(client_socket, message_status);
        printf("%s\n", message_status.status_message);
      
        // Sending the response
        send_server_request(buffer, client_socket);

        printf("%s\n", message_status.status_message);

        // CLose the socket
        close(client_socket);
        printf("Client disconnected.\n\n");
  }


  return 0;
}

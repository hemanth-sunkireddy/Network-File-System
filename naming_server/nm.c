#include "headers.h"

char *ip = "127.0.0.1";
int port = PORT;
char message[1024];
int main(){

  int nm_server_socket, ss_socket;
  struct sockaddr_in nm_server_address, ss_address;
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
  nm_server_address.sin_port = port;
  nm_server_address.sin_addr.s_addr = inet_addr(ip);
  
  // Bind the server socket.
  int bind_check;
  bind_check = bind(nm_server_socket, (struct sockaddr*)&nm_server_address, sizeof(nm_server_address));
  if (bind_check < 0){
    perror("Bind error");
    exit(1);
  }
  else{
    printf("Binded to the port number: %d\n", port);
  }

  while(1){

        // Listen server
        listen(nm_server_socket, 5);
        printf("Listening...\n");

        printf("Wait till the client connects:\n");

        // Accept the ss_socket 
        address_size = sizeof(ss_address);
        ss_socket = accept(nm_server_socket, (struct sockaddr*)&ss_address, &address_size);
        printf("Client connected.\n");
        printf("Wait till the client sends the request:\n");

        acknowledgmentMessage message_status;


         

        // Recieve the request
        message_status = receive_initialized_ss_info(ss_socket, message_status);
        printf("%s\n", message_status.status_message);
      
        
        // Sending the response
        //send_server_request(message_status, ss_socket);

        // CLose the socket
        close(ss_socket);
        printf("Client disconnected.\n\n");
  }


  return 0;
}

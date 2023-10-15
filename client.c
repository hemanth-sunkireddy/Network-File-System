#include "headers.h"


void send_request(char* buffer, int sock){
        bzero(buffer, MAX_LENGTH);
        printf("Enter your request and wait till the server sends response back:");
        fgets(buffer, MAX_LENGTH, stdin);
        int length_of_buffer = strlen(buffer);
        buffer [ length_of_buffer - 1 ] = '\0'; 
        printf("Client: %s\n", buffer);
        send(sock, buffer, strlen(buffer), 0);
}

int main(){

  int sock;
  struct sockaddr_in client_address;
  socklen_t client_address_size;
  char buffer[MAX_LENGTH];

  

   while(1){

          // Creating socket for connecting to server.
          sock = socket(AF_INET, SOCK_STREAM, 0);
          if (sock < 0){
            perror("Socket error");
            exit(1);
          }
          else{
            printf("TCP SERVER SOCKET CREATED.\n");
          }

          memset(&client_address, '\0', sizeof(client_address));
          client_address.sin_family = AF_INET;
          client_address.sin_port = port;
          client_address.sin_addr.s_addr = inet_addr(ip);

          // Connect to the server
          connect(sock, (struct sockaddr*)&client_address, sizeof(client_address));
          printf("Connected to the server.\n");

        
          // Send the request to server
          send_request(buffer, sock);


          // Recieve the response from server
          printf("Wait till the response from server:\n");
          bzero(buffer, MAX_LENGTH);
          recv(sock, buffer, sizeof(buffer), 0);
          printf("Server: %s\n", buffer);



          // Close the socket.
          close(sock);
          printf("Disconnected from the server.\n");
          sleep(1);

  }
  return 0;

}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h> 
#include<errno.h>
#include <dirent.h>
#include <limits.h>
#include <time.h>

#define PORT 8000
#define MAX_LENGTH 1024


char *ip = "127.0.0.1";
int port = PORT;
char message[MAX_LENGTH];

int main(){

  int client_socket;
  struct sockaddr_in server_address;
  socklen_t client_address_size;
  char buffer[MAX_LENGTH];

  

   while(1){
          
          // Creating socket for connecting to server.
          client_socket = socket(AF_INET, SOCK_STREAM, 0);
          if (client_socket < 0){
            perror("Socket error");
            exit(1);
          }
          else{
            printf("TCP CLIENT SOCKET CREATED.\n");
          }
          printf("Client socket Number: %d\n", client_socket);

          memset(&server_address, '\0', sizeof(server_address));
          server_address.sin_family = AF_INET;
          server_address.sin_port = port;
          server_address.sin_addr.s_addr = inet_addr(ip);

          printf("Server Port: %d\n", server_address.sin_port);
          printf("Server address: %s\n", inet_ntoa(server_address.sin_addr));

          // Connect to the server
          int connection = connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address));
          printf("Connection status: %d\n", connection);
          if ( connection == 0 ){
            printf("Connected to the server.\n\n");
            struct sockaddr_in local_address;
            socklen_t local_address_len = sizeof(local_address);
            if (getsockname(client_socket, (struct sockaddr*)&local_address, &local_address_len) == 0) {
                printf("Client IP Address: %s\n", inet_ntoa(local_address.sin_addr));
                printf("Client Port Number: %d\n", ntohs(local_address.sin_port));
            } else {
                perror("getsockname");
            }
          }
          strcpy(buffer, "THIRD MESSAGE FROM CLIENT.");
          send(client_socket, buffer, MAX_LENGTH, 0);


          // Close the socket.
          close(client_socket);
          printf("Disconnected from the server.\n\n");
          sleep(5);

  }
  return 0;

}

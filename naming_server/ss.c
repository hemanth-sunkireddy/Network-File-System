#include "headers.h"

char *ip = "127.0.0.1";
int port = PORT;
char message[MAX_LENGTH];


void send_client_request(int client_socket) 
{
    SS_Info ss;
    ss.ip[20]; 
    strcpy(ss.ip, "127.0.0.1");
    ss.NM_port = 5000;
    ss.client_port = 6000;
    ss.storage_server_number = 1;
    ss.paths_accessible[MAX_LENGTH];
    strcpy(ss.paths_accessible[0].path, "/home/Desktop");
    printf("hello\n");
    send(client_socket, &ss, sizeof(SS_Info), 0);
}

int main(){

  int client_socket;
  struct sockaddr_in client_address;
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

          memset(&client_address, '\0', sizeof(client_address));
          client_address.sin_family = AF_INET;
          client_address.sin_port = port;
          client_address.sin_addr.s_addr = inet_addr(ip);

          // Connect to the server
          connect(client_socket, (struct sockaddr*)&client_address, sizeof(client_address));
          printf("Connected to the server.\n\n");

          printf("hello\n");
          // Send the request to server
          send_client_request(client_socket);
          printf("hello\n");
          printf("Sent successfully\n");

          // Recieve the response from server
          //printf("\nWait till the response from server:\n");
          //receive_server_request(client_socket);


          // Close the socket.
          close(client_socket);
          printf("Disconnected from the server.\n\n");
          sleep(1);

  }
  return 0;

}

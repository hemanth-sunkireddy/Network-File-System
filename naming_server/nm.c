// #include "headers.h"

// char *ip = "127.0.0.1";
// int port = PORT;
// char message[1024];
// int main(){

//   SS_Info ssx[MAX_STORAGE_SERVERS];
//   // for (int i = 0; i < 4; i++ ){ 
//   //   ssx[i].num_of_files = 4; 
//   // }
//   for (int i = 0; i < MAX_STORAGE_SERVERS; i++){
//     ssx[i].SS_port = PORT + i;
//     printf("Storage server %d number connected to port number %d\n", i, PORT + i);
//   }
//   int nm_server_socket, ss_socket;
//   struct sockaddr_in nm_server_address, ss_address;
//   socklen_t address_size;
//   char buffer[MAX_LENGTH];
  
//   // Activate socket for the nm_server.
//   nm_server_socket = socket(AF_INET, SOCK_STREAM, 0);
//   if (nm_server_socket < 0){
//     perror("Socket error");
//     exit(1);
//   }
//   else{
//     printf("TCP nm_server socket created.\n");
//   }

//   memset(&nm_server_address, '\0', sizeof(nm_server_address));
//   nm_server_address.sin_family = AF_INET;
//   nm_server_address.sin_port = NAMING_SERVER_MAIN_PORT;
//   nm_server_address.sin_addr.s_addr = inet_addr(ip);
  
//   // Bind the server socket.
//   int bind_check;
//   bind_check = bind(nm_server_socket, (struct sockaddr*)&nm_server_address, sizeof(nm_server_address));
//   if (bind_check < 0){
//     perror("Bind error");
//     exit(1);
//   }
//   else{
//     printf("Binded to the port number: %d\n", NAMING_SERVER_MAIN_PORT);
//   }

//   while(1){

//         // Listen server
//         listen(nm_server_socket, 5);
//         printf("Listening...\n");

//         printf("Wait till the client connects:\n");

//         // Accept the ss_socket 
//         address_size = sizeof(ss_address);
//         ss_socket = accept(nm_server_socket, (struct sockaddr*)&ss_address, &address_size);
//         printf("Client connected.\n");
//         printf("Wait till the client sends the request:\n");

//         acknowledgmentMessage message_status;



//         fileNameAndOperation client_operation_number_path_name; 
//         recv(ss_socket, &client_operation_number_path_name, sizeof(fileNameAndOperation), 0);
        
//         printf("Operation number client asked:%d\n", client_operation_number_path_name.operation_number);
//         printf("PATH CLIENT ASKED: %s\n", client_operation_number_path_name.name_of_file_or_folder);
//         // If operation number == create or delete
//         // Suppose path found in first storage server. 

// // int port_need_to_connect = ssx[0].SS_port;
// //         int client_socket;
// //         struct sockaddr_in client_address;
// //         socklen_t client_address_size;
// //         client_socket = socket(AF_INET, SOCK_STREAM, 0);
// //           if (client_socket < 0){
// //             perror("Socket error");
// //             exit(1);
// //           }
// //           else{
// //             printf("TCP CLIENT SOCKET CREATED for NS.\n");
// //           }        

//           // memset(&client_address, '\0', sizeof(client_address));
//           // client_address.sin_family = AF_INET;
//           // client_address.sin_port = ssx[0].SS_port;
//           // client_address.sin_addr.s_addr = inet_addr(ip);

//           // Connect to the server
//         //   connect(client_socket, (struct sockaddr*)&client_address, sizeof(client_address));
//         //   printf("Connected to the server .\n\n");
          
//         // strcpy(message_status.status_message, "Just routing successful.");
//         //obtain_ss_info(ss_socket,message_status,)
//         //send(client_socket, message_status.status_message, MAX_LENGTH, 0);
         

//         // // Recieve the request
//         message_status = obtain_ss_info(ssx,ss_socket, message_status,client_operation_number_path_name);
//         // printf("%s\n", message_status.status_message);
      
        
//         // // Sending the response
//         // //send_server_request(message_status, ss_socket);

//         // // CLose the socket
//         // close(ss_socket);
//         // printf("Client disconnected.\n\n");
//   }


//   return 0;
// }

#include "headers.h"

char *ip = "127.0.0.1";
int port = PORT;
char message[1024];
int main(){

  SS_Info ssx[MAX_STORAGE_SERVERS];
  for (int i = 0; i < CURRENT_STORAGE_SERVERS; i++){
    ssx[i].SS_port = PORT + i;
    strcpy(ssx[i].ip,ip);
    ssx[i].storage_server_number=i;
    printf("Storage server %d number connected to port number %d\n", i, PORT + i);
  }
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



        fileNameAndOperation client_operation_number_path_name; 
        recv(ss_socket, &client_operation_number_path_name, MAX_LENGTH, 0);
        
        printf("Operation number client asked:%d\n", client_operation_number_path_name.operation_number);
        printf("PATH CLIENT ASKED: %s\n", client_operation_number_path_name.name_of_file_or_folder);
        // If operation number == create or delete
        // Suppose path found in first storage server. 

// int port_need_to_connect = ssx[0].SS_port;
//         int client_socket;
//         struct sockaddr_in client_address;
//         socklen_t client_address_size;
//         client_socket = socket(AF_INET, SOCK_STREAM, 0);
//           if (client_socket < 0){
//             perror("Socket error");
//             exit(1);
//           }
//           else{
//             printf("TCP CLIENT SOCKET CREATED for NS.\n");
//           }        

          // memset(&client_address, '\0', sizeof(client_address));
          // client_address.sin_family = AF_INET;
          // client_address.sin_port = ssx[0].SS_port;
          // client_address.sin_addr.s_addr = inet_addr(ip);

          // Connect to the server
        //   connect(client_socket, (struct sockaddr*)&client_address, sizeof(client_address));
        //   printf("Connected to the server .\n\n");
          
        // strcpy(message_status.status_message, "Just routing successful.");
        //obtain_ss_info(ss_socket,message_status,)
        //send(client_socket, message_status.status_message, MAX_LENGTH, 0);
         

        // // Recieve the request
        message_status = obtain_ss_info(ssx,ss_socket, message_status,client_operation_number_path_name);
        // printf("%s\n", message_status.status_message);
      
        
        // // Sending the response
        // //send_server_request(message_status, ss_socket);

        // // CLose the socket
        // close(ss_socket);
        // printf("Client disconnected.\n\n");
  }


  return 0;
}


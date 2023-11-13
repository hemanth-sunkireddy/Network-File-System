
#include "headers.h"


int main(){
      


    char ip[100] = "127.0.0.1";

    int client_socket;
    struct sockaddr_in client_address;
    socklen_t client_address_size;
    char buffer[MAX_LENGTH];

          
          // Creating socket for connecting to Naming server.
          client_socket = socket(AF_INET, SOCK_STREAM, 0);
          if (client_socket < 0){
            perror("Socket error");
            exit(1);
          }
          else{
            printf("TCP CLIENT SOCKET CREATED for NS.\n");
          }

          memset(&client_address, '\0', sizeof(client_address));
          client_address.sin_family = AF_INET;
          client_address.sin_port = port;
          client_address.sin_addr.s_addr = inet_addr(ip);

          // Connect to the server
          connect(client_socket, (struct sockaddr*)&client_address, sizeof(client_address));
          printf("Connected to the server.\n\n");

        
          // Send the request to server
           fileNameAndOperation file_or_folder_details;
           Info(&file_or_folder_details);
           send(client_socket, &file_or_folder_details, sizeof(file_or_folder_details), 0);

     
          
          // Recieve the response from server
          printf("\nWait till the response from server:\n");
            

          if(file_or_folder_details.operation_number==3 ||file_or_folder_details.operation_number==4 ||file_or_folder_details.operation_number==8 ||file_or_folder_details.operation_number==9){
             char message_status[MAX_LENGTH];
            recv(client_socket, message_status, MAX_LENGTH, 0);
             
            printf("OPERATION STATUS:%s\n", message_status);
            close(client_socket);
            printf("Disconnected from the Naming Server.\n");
          }
          else{ //1,2,5,6,7
             //
             NS_reply reply;
             recv(client_socket, &reply, sizeof(reply), 0);
              close(client_socket);
             printf("Disconnected from the Naming Server.\n");
             // Close the socket.
            
             //create socket for Storage Server
             strcpy(ip, reply.IP);
            port = reply.port;

            int client_socket;
            struct sockaddr_in client_address;
            socklen_t client_address_size;
            //

             // Creating socket for connecting to Naming server.
            client_socket = socket(AF_INET, SOCK_STREAM, 0);
            if (client_socket < 0){
                perror("Socket error");
                exit(1);
            }
            else{
                printf("TCP CLIENT SOCKET CREATED for SS.\n");
            }

            memset(&client_address, '\0', sizeof(client_address));
            client_address.sin_family = AF_INET;
            client_address.sin_port = port;
            client_address.sin_addr.s_addr = inet_addr(ip);

            // Connect to the SS
            connect(client_socket, (struct sockaddr*)&client_address, sizeof(client_address));
            printf("Connected to the SS\n");


             send(client_socket, &file_or_folder_details, sizeof(file_or_folder_details), 0);
            printf("\nWait till the response from SS\n");

            
            acknowledgmentMessage ack_fromSS;
            int flag=1;
            while(1){
             recv(client_socket,&ack_fromSS ,sizeof(ack_fromSS), 0);
             //print 
            //              char status_message[MAX_LENGTH];
            // Packet file_or_folder_content[MAX_LENGTH];
            // folderInformation folder_information;
                 printf("%s",ack_fromSS.status_message);

                 if(strcmp(ack_fromSS.status_message,"STOP")==0){
                    flag=0;
                 }

                 //--** PRINT folder_information
                 //--** PRINT  file_or_folder_content[MAX_LENGTH]
            
            
            }

            //STOP
            close(client_socket);
            printf("Disconnected from the Storage Server.\n");

            




          }        
 

          
         
         

  
  return 0;

}


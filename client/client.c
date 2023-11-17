
#include "headers.h"



void Info(fileNameAndOperation *file_or_folder_details){


        // fileNameAndOperation file_or_folder_details;

        // Displaying all options in the terminal of the client side.
        printf("Choose one number from below:\n");
        printf("-------------------------------\n");
        printf("1 - Reading a File\n");
        printf("2 - Writing a File\n");
        printf("3 - Deleting a File\n");
        printf("4 - Creating a File\n");
        printf("5 - Getting Additional information of File \n");
        printf("6 - Getting Additional information of Folder\n");
        printf("7 - Listing all Files and Folders\n");
        printf("8 - Deleting a Folder\n");
        printf("9 - Creating a Folder\n");
        printf("10- Copy Folder\n");
        printf("11- Copy File\n");
        printf("-------------------------------\n");

        printf("Enter the number:");
        int option_choosing_number; 
        scanf("%d", &(file_or_folder_details->operation_number));

        if ( file_or_folder_details->operation_number <= 5){ 
                printf("Enter the file name: ");
        }
        else{
                printf("Enter the Folder name: ");
        }
        scanf("%s", file_or_folder_details->name_of_file_or_folder);
}

int main(){
      


    char ip[100] = "127.0.0.1";
    int port=4000; // = PORT;

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
           send(client_socket, &file_or_folder_details, sizeof(file_or_folder_details), 0); ////

     
          
          // Recieve the response from server
          printf("\nWait till the response from server:\n");
          //   typedef struct source_dest{
          //     char source_path[1024];
          //     char destination_path[1024];
          // }source_dest;

          if(file_or_folder_details.operation_number==3 ||file_or_folder_details.operation_number==4 ||file_or_folder_details.operation_number==8 ||file_or_folder_details.operation_number==9 ||file_or_folder_details.operation_number==10 ||file_or_folder_details.operation_number==11 ){
            
            if(file_or_folder_details.operation_number==10 ||file_or_folder_details.operation_number==11){
                source_dest path;
                printf("Enter Source Path\n");
                scanf("%s", path.source_path);
                printf("Enter Destination Path\n");
                scanf("%s", path.destination_path);
                send(client_socket, &path, sizeof(path), 0);  //send path after sending  ,file_or_folder_details
              
            }
            
             char message_status[MAX_LENGTH];
            recv(client_socket, message_status, MAX_LENGTH, 0);
             
            printf("OPERATION STATUS:%s\n", message_status);
            close(client_socket);
            printf("Disconnected from the Naming Server.\n");
          }
          else{ //1,2,5,6,7
             //
             NS_reply reply;
             char message[1024];
             recv(client_socket, message, 1024, 0);
             printf("MESSAGE FROM NAMING SERVER: %s\n", message);
              close(client_socket);
             printf("Disconnected from the Naming Server.\n");
             // Close the socket.
            //************
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

            
            client_receving_data(client_socket,file_or_folder_details); //does communicate with SS

           
            close(client_socket);
            printf("Disconnected from the Storage Server.\n");

            


 

          }        
          
        

  
  return 0;

}


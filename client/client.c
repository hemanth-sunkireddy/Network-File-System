#include "../headers.h"


char *ip = "127.0.0.1";
int port = PORT;
char message[MAX_LENGTH];


void Info(fileNameAndOperation *file_or_folder_detail){

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
        printf("-------------------------------\n");

        printf("Enter the number:");
        int option_choosing_number; 
        scanf("%d", file_or_folder_details->operation_number);

        if ( file_or_folder_details->operation_number <= 5){ 
                printf("Enter the file name: ");
        }
        else{
                printf("Enter the Folder name: ");
        }
        scanf("%s", file_or_folder_details->name_of_file_or_folder);
        
        // send(client_socket, &file_or_folder_details, MAX_LENGTH, 0);
}
typedef struct fileNameAndOperation{
    int operation_number; 
    char name_of_file_or_folder[MAX_LENGTH];
}fileNameAndOperation;
typedef struct acknowledgmentMessage{
    int operation_number;
    char status_message[MAX_LENGTH];
    Packet file_or_folder_content[MAX_LENGTH];
    folderInformation folder_information;
}acknowledgmentMessage;

typedef struct NS{
    int PORT;
    int IP;
}NS_reply;


int main(){

  
  

   while(1){
    int client_socket;
    struct sockaddr_in client_address;
    socklen_t client_address_size;
    char buffer[MAX_LENGTH];

          
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

        
          // Send the request to server
           fileNameAndOperation file_or_folder_details;
           Info(&file_or_folder_details);
           send(client_socket, &file_or_folder_details, MAX_LENGTH, 0);

        //   int op=send_client_request(client_socket);
        ///////

        // Recieve the response from server
          printf("\nWait till the response from server:\n");
    //  receive_server_request(client_socket);
          if(file_or_folder_details.operation_number==3 ||file_or_folder_details.operation_number==4 ||file_or_folder_details.operation_number==8 ||file_or_folder_details.operation_number==9){
             char message_status[MAX_LENGTH];
            recv(client_socket, message_status, MAX_LENGTH, 0);
            printf("OPERATION STATUS:%s\n", message_status);
          }
          else{//1,2,5,6,7
             //
             NS_reply reply;
             recv(client_socket, reply, sizeof(reply), 0);
              close(client_socket);
             printf("Disconnected from the Naming Server.\n");
             // Close the socket.


             //create socket for Storage Server







          }

        
         

          
         
 

          
         
         

  }
  return 0;

}

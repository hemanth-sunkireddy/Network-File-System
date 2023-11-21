
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
        else if(file_or_folder_details->operation_number <= 9 && file_or_folder_details->operation_number>=6){
                printf("Enter the Folder name: ");
        }
        scanf("%s", file_or_folder_details->name_of_file_or_folder);
}

int main(){
      


    char ip[100] = "127.0.0.1";
    int port=4000; // = PORT;

    acknowledgmentMessage message_status;  
    int err_code=0;

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
           send(client_socket, &file_or_folder_details, MAX_LENGTH, 0); 


          // Copying the files and folders details updation. 
          Copy_source_dest src_dest_path_oper;
          if(file_or_folder_details.operation_number == 10 || file_or_folder_details.operation_number == 11){
                char destination_path[MAX_LENGTH];
                printf("Enter Destination Path\n");
                scanf("%s", destination_path);
                int send_status1 = send(client_socket,destination_path , MAX_LENGTH, 0);  //send destination path to the storage server. 
                printf("sending status: %d\n", send_status1);
            }


          // Recieve the response from server
          printf("\nWait till the response from server:\n");

          if(file_or_folder_details.operation_number==3 ||file_or_folder_details.operation_number==4 ||file_or_folder_details.operation_number==8 ||file_or_folder_details.operation_number==9){
            
        
            char message_status1[MAX_LENGTH];
            recv(client_socket, message_status1, MAX_LENGTH, 0);
            strcpy(message_status.status_message,message_status1);
            printf("OPERATION STATUS:%s\n", message_status.status_message);
            
            close(client_socket);
            printf("Disconnected from the Naming Server.\n");
          }
          else if ( file_or_folder_details.operation_number == 10 || file_or_folder_details.operation_number == 11 ){ 
            printf("MESSAGE FROM STORAGE SERVER: COPYING FILE SUCCESSFUL.\n");
            close(client_socket);
            printf("Disconnected from the Naming server.\n");
          }
          else{ //1,2,5,6,7
             
             NS_reply reply;
             recv(client_socket, &reply, MAX_LENGTH, 0);
             printf("MESSAGE FROM NAMING SERVER %d %d\n", reply.port,reply.SS_no);
              close(client_socket);
             printf("Disconnected from the Naming Server.\n");
             


             //create socket for Storage Server
             strcpy(ip, reply.IP);
             port = reply.port;

            int client_socket;
            struct sockaddr_in client_address;
            socklen_t client_address_size;

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


            send(client_socket, &file_or_folder_details, MAX_LENGTH, 0);
            printf("\nWait till the response from SS\n");

            printf("Data sent success to storage server except write file details.\n");

            message_status=client_receving_data(client_socket,file_or_folder_details,message_status); 

           
            close(client_socket);
            printf("Disconnected from the Storage Server.\n");

          }        

            if(strcmp(message_status.status_message,"ERROR CREATING FILE")==0){         //Create file
                err_code=1;
            }
            else if(strcmp(message_status.status_message,"ERROR GETTING FULL PATH")==0){     // Delete file
                err_code=2;
            }
            else if(strcmp(message_status.status_message,"ERROR DELETING FILE")==0){     // Delete file
                err_code=3;
            }
            else if(strcmp(message_status.status_message,"COULD NOT OPEN FILE")==0){     // Read file
                err_code=4;
            }
            else if(strcmp(message_status.status_message,"ERROR OPENING FILE")==0){     // Write file
                err_code=4;
            }
            else if(strcmp(message_status.status_message,"Error getting file information")==0){     // Add. info of file file
                err_code=5;
            }
            else if(strcmp(message_status.status_message,"ERROR OPENING FILE")==0){     // Copy file
                err_code=4;
            }
            else if(strcmp(message_status.status_message,"Folder already exists")==0){     // Create folder
                err_code=6;
            }
            else if(strcmp(message_status.status_message,"Permission denied to create folder")==0){     // Create folder
                err_code=7;
            }
            else if(strcmp(message_status.status_message,"The parent folder doesnot exist")==0){     // Create folder
                err_code=8;
            }
            else if(strcmp(message_status.status_message,"Failed to create folder")==0){     // Create folder
                err_code=9;
            }
            else if(strcmp(message_status.status_message,"ERROR OPENING FOLDER. NO SUCH DIRECTORY")==0){     // Delete folder
                err_code=10;
            }
            else if(strcmp(message_status.status_message,"ERROR DELETING FOLDER")==0){     // Delete folder
                err_code=11;
            }
            else if(strcmp(message_status.status_message,"Error getting folder")==0){     // Additional information of folder
                err_code=12;
            }
            else if(strcmp(message_status.status_message,"Error opening folder")==0){     // Additional information of folder
                err_code=10;
            }
            else if(strcmp(message_status.status_message,"Error getting parent folder information")==0){     // Additional information of folder
                err_code=13;
            }
            else if(strcmp(message_status.status_message,"Error opening directory")==0){     // Folder read(Listing all files and folders)
                err_code=10;
            }
            if(err_code!=0){
                printf("Error code: %d\n",err_code);
            } 
          
        

  
  return 0;

}


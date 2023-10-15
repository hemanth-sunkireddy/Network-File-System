#include "headers.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h> 
extern char *message;
void receive_client_request(char* buffer, int client_socket)
{   
    //send(client_socket, "12345678", strlen("12345"), 0);
    
    bzero(buffer, MAX_LENGTH);
    char file_name[MAX_LENGTH];
    char file_operation[MAX_LENGTH];
    bzero(file_name, MAX_LENGTH); bzero(file_operation, MAX_LENGTH);
    recv(client_socket, file_name, MAX_LENGTH, 0);
    printf("servere:%s", file_name);

    

    recv(client_socket, file_operation, MAX_LENGTH, 0);
    printf("server: %s\n", file_operation);

    

    //read a file :

    if (strstr(file_operation, "read") != NULL) {
         // Open the file.
            FILE *fp = fopen(file_name, "r");

            // Check if the file opened successfully.
            if (fp == NULL) {
                printf("Could not open file.\n");
                return ;
            }

            // Read the file line by line.
            char line[1024];
            while (fgets(line, sizeof(line), fp) != NULL) {
                // printf("%ld  %s ",sizeof(line), line);
                printf("%s", line);
            }

            // Close the file.
            fclose(fp);
        
    }
        

    //write a file :
    else  if(strstr(file_operation,"write")!=NULL){
                // char mode[200];
                // printf("overwrite: 1 , append: 2");
                // scanf("%s", mode);
                // send(sock, mode, strlen(mode), 0);

                printf("Input string ot write:\n");
                char *input = NULL;
                size_t n = 50;
                ssize_t read_bytes =getline(&input, &n, stdin);
                input[strcspn(input, "\n")]='\0';	
            // char input[MAX_LENGTH];
            // recv(client_socket, input, MAX_LENGTH, 0);
            // printf("SERVER:%s\n", input);

            // Write the string to a file
            FILE *file = fopen(file_name, "w");
            if (file == NULL) {
                perror("Error opening file");
                return ;
            }
            fprintf(file, "%s\n", input);
            fclose(file);


        	
    }
    //delete a file :
    else if(strstr(file_operation,"delete")!=NULL){
        	

            //const char *file_path = file_name; // Replace with the path to the file you want to delete
            char file_path[200];
            if (realpath(file_name, file_path) != NULL) {
                printf("Full file path: %s\n", file_path);
            } else {
                perror("Error getting full path");
                return ; // Return an error code to indicate failure
            }

            if (remove(file_path) == 0) {
                printf("File deleted successfully.\n");
                strcpy(message,"File deleted successfully.");
            } else {
                perror("Error deleting file");
                strcpy(message,"Error deleting file");
            }
            //send(client_socket, message, strlen(message), 0);

        	
    }
    //create a file
    else if(strstr(file_operation,"create")!=NULL){
            
            // Open the file for writing (creates a new file if it doesn't exist)
            FILE *file = fopen(file_name, "w");
            
            if (file == NULL) {
                perror("Error creating file");
                return ; // Return an error code to indicate failure
            }
            
            // Close the file
            fclose(file);
            
            printf("File created successfully: %s\n", file_name);

    

    }

    



    send(client_socket, "hi", strlen("hi"), 0);


}
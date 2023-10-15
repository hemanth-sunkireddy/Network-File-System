#include "headers.h"
extern char *message;
void send_client_request(char* buffer, int sock){
        char file_name[MAX_LENGTH];
        char file_operation[MAX_LENGTH];

        bzero(buffer, MAX_LENGTH);
        printf("Enter the file name:");
        scanf("%s", file_name);
        send(sock, file_name, strlen(file_name), 0);

        // char n[100];
        // recv(sock, n, sizeof(n), 0);
        // printf("n %s\n",n);

        printf("Enter the operation you want");
        scanf("%s", file_operation);
        //printf("%s %s\n", file_name, file_operation);
        // fgets(buffer, MAX_LENGTH, stdin);
        // int length_of_buffer = strlen(buffer);
        // buffer [ length_of_buffer - 1 ] = '\0'; 
        // printf("Client: %s\n", buffer);
        
        
        send(sock, file_operation, strlen(file_operation), 0);

        // if(strstr(file_operation,"write")!=NULL){
        //         // char mode[200];
        //         // printf("overwrite: 1 , append: 2");
        //         // scanf("%s", mode);
        //         // send(sock, mode, strlen(mode), 0);

        //         printf("Input string ot write:\n");
        //         char *input = NULL;
        //         size_t n = 50;
        //         ssize_t read_bytes =getline(&input, &n, stdin);
        //         input[strcspn(input, "\n")]='\0';

        //         send(sock, input, strlen(input), 0);


        // }
}

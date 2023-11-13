#include "../headers.h"

acknowledgmentMessage writing_the_file(char file_name[2*MAX_LENGTH], acknowledgmentMessage message_status){
      //write a file :
            //     // char mode[200];
            //     // printf("overwrite: 1 , append: 2");
            //     // scanf("%s", mode);
            //     // send(sock, mode, strlen(mode), 0);

            //     printf("Input string ot write:\n");
            //     char *input = NULL;
            //     size_t n = 50;
            //     ssize_t read_bytes =getline(&input, &n, stdin);
            //     input[strcspn(input, "\n")]='\0';	
            // // char input[MAX_LENGTH];
            // // recv(client_socket, input, MAX_LENGTH, 0);
            // // printf("SERVER:%s\n", input);

            // // Write the string to a file
            // FILE *file = fopen(file_name, "w");
            // if (file == NULL) {
            //     perror("Error opening file");
            //     strcpy(message_status.status_message, "ERROR OPENING FILE");
            //     return message_status;
            // }
            // else{
            //     fprintf(file, "%s\n", input);
            //     strcpy(message_status.status_message,"OPENED FILE SUCCESSFULLY");
            //     fclose(file);
            //     printf("File write successfully\n");
            //     return message_status;
            // }
            printf("Input string to write:\n");

    char *input = NULL;
    size_t n = 0;
    ssize_t read_bytes = getline(&input, &n, stdin);
    if (read_bytes == -1) {
        perror("Error reading input");
        strcpy(message_status.status_message, "ERROR READING INPUT");
        return message_status;
    }
    input[strcspn(input, "\n")] = '\0';

    // Write the string to a file
    FILE *file = fopen(file_name, "w");
    if (file == NULL) {
        perror("Error opening file");
        strcpy(message_status.status_message, "ERROR OPENING FILE");
        free(input);
        return message_status;
    }

    // Write the input string with newline character.
    fprintf(file, "%s\n", input);

    // Close the file and free the input buffer.
    fclose(file);
    free(input);

    strcpy(message_status.status_message, "WROTE FILE SUCCESSFULLY");
    printf("File write successfully\n");
    return message_status;
}
#include "headers.h"

void receive_client_request(int client_socket)
{   
    fileNameAndOperation file_or_folder_details;

    recv(client_socket, &file_or_folder_details, MAX_LENGTH, 0);
    printf("client Asked for this file / folder: %s\n", file_or_folder_details.name_of_file_or_folder);

    int operation_number = file_or_folder_details.operation_number; 
    printf("The operation number he had choosen.:%d\n", operation_number);
    if ( operation_number == 1 ) 
    { 
        reading_the_file(file_or_folder_details.name_of_file_or_folder);      
    }
    else if ( operation_number == 2 ) 
    { 
        writing_the_file(file_or_folder_details.name_of_file_or_folder);
    }
    else if ( operation_number == 3 ) 
    {
        deleting_the_file(file_or_folder_details.name_of_file_or_folder);
    }
    else if ( operation_number == 4 ) 
    { 
        creating_the_file(file_or_folder_details.name_of_file_or_folder);
    }
    else if ( operation_number == 5 ) 
    { 
        additional_information_of_file(file_or_folder_details.name_of_file_or_folder);
    }  
    else if ( operation_number == 6 )
    { 
        additional_information_of_folder(file_or_folder_details.name_of_file_or_folder);
    }
    else if ( operation_number == 7 ) 
    {
        writing_the_folder(file_or_folder_details.name_of_file_or_folder);
    }
    else if ( operation_number == 8 ) 
    { 
        deleting_the_folder(file_or_folder_details.name_of_file_or_folder);
    }
    else if ( operation_number == 9 )
    { 
        creating_the_folder(file_or_folder_details.name_of_file_or_folder);
    }
    else if ( operation_number == 10 )
    {
        listing_all_files_and_folders(file_or_folder_details.name_of_file_or_folder);
    }

    //read a file :
    // if (strstr(file_operation, "read") != NULL) {
    //         // Open the file.
    //         FILE *file_pointer= fopen(file_name, "r");

    //         // Check if the file opened successfully.
    //         if (file_pointer == NULL) {
    //             printf("Could not open file.\n");
    //             return ;
    //         }
    //         else{
    //             printf("File opened successfully\n");
    //         }

    //         // Read the file contents.
    //         fseek(file_pointer, 0L, SEEK_END); 
    //         long long int length_of_the_file= ftell(file_pointer); 
    //         printf("Total size of the file: %lld\n", length_of_the_file);

    //         Packet file_content[length_of_the_file];
    //         char file_data[MAX_LENGTH];
    //         int temp_count = 0;

    //         printf("File data:\n");
    //         while (fgets(file_data, sizeof(file_data), file_pointer) != NULL) {
    //             // file_content[temp_count].total_chunks = length_of_the_file;
    //             // strcpy(file_content[temp_count++].data, file_data);
    //             printf("%s", file_data);
    //         }

    //         // Close the file.
    //         fclose(file_pointer);
    //         printf("File read successfully\n");

    //         for ( int i = 0; i < length_of_the_file; i++ ) { 
    //             printf("%s ", file_content[i].data);
    //         }
        
    // }
        

    // //write a file :
    // else  if(strstr(file_operation,"write")!=NULL){
    //             // char mode[200];
    //             // printf("overwrite: 1 , append: 2");
    //             // scanf("%s", mode);
    //             // send(sock, mode, strlen(mode), 0);

    //             printf("Input string ot write:\n");
    //             char *input = NULL;
    //             size_t n = 50;
    //             ssize_t read_bytes =getline(&input, &n, stdin);
    //             input[strcspn(input, "\n")]='\0';	
    //         // char input[MAX_LENGTH];
    //         // recv(client_socket, input, MAX_LENGTH, 0);
    //         // printf("SERVER:%s\n", input);

    //         // Write the string to a file
    //         FILE *file = fopen(file_name, "w");
    //         if (file == NULL) {
    //             perror("Error opening file");
    //             return ;
    //         }
    //         fprintf(file, "%s\n", input);
    //         fclose(file);


        	
    // }
    // //delete a file :
    // else if(strstr(file_operation,"delete")!=NULL){
        	

    //         //const char *file_path = file_name; // Replace with the path to the file you want to delete
    //         char file_path[200];
    //         if (realpath(file_name, file_path) != NULL) {
    //             printf("Full file path: %s\n", file_path);
    //         } else {
    //             perror("Error getting full path");
    //             return ; // Return an error code to indicate failure
    //         }

    //         if (remove(file_path) == 0) {
    //             printf("File deleted successfully.\n");
    //             strcpy(message,"File deleted successfully.");
    //         } else {
    //             perror("Error deleting file");
    //             strcpy(message,"Error deleting file");
    //         }
    //         //send(client_socket, message, strlen(message), 0);

        	
    // }
    // //create a file
    // else if(strstr(file_operation,"create")!=NULL){
            
    //         // Open the file for writing (creates a new file if it doesn't exist)
    //         FILE *file = fopen(file_name, "w");
            
    //         if (file == NULL) {
    //             perror("Error creating file");
    //             return ; // Return an error code to indicate failure
    //         }
            
    //         // Close the file
    //         fclose(file);
            
    //         printf("File created successfully: %s\n", file_name);

    // }

    



    send(client_socket, "hi", strlen("hi"), 0);


}
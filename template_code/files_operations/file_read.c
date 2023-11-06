#include "../headers.h"

acknowledgmentMessage reading_the_file(char file_name[MAX_LENGTH], acknowledgmentMessage message_status){

            // Open the file.
            printf("%s\n", file_name);

            FILE *file_pointer= fopen(file_name, "r");
           

            // Server folder is the folder from where the reading is happening.

            // Check if the file opened successfully.
            if (file_pointer == NULL) {
                printf("Could not open file.\n");
                strcpy ( message_status.status_message, "COULD NOT OPEN FILE");
                return message_status;
            }
            else{
                printf("File opened successfully\n");
                strcpy ( message_status.status_message, "OPENED SUCCESSFULLY");
            }


            // Read the file contents.

            Packet file_content[MAX_LENGTH];
            char file_data2[MAX_LENGTH];
            int temp_count = 0;


            while (fgets(file_data2, MAX_LENGTH, file_pointer) != NULL) {
                strcpy(file_content[temp_count++].data, file_data2);
            }

            file_content[0].total_chunks = temp_count;

            // Close the file.
            fclose(file_pointer);
            printf("File Closed successfully\n");


            memcpy(message_status.file_or_folder_content, file_content, MAX_LENGTH * MAX_LENGTH);

            return message_status;
}

// #include "../headers.h"

// acknowledgmentMessage reading_the_file(char file_name[2*MAX_LENGTH], acknowledgmentMessage message_status){

//             // Open the file.
//             printf("%s\n", file_name);

//             FILE *file_pointer= fopen(file_name, "r");
           


//             // Check if the file opened successfully.
//             if (file_pointer == NULL) {
//                 printf("Could not open file.\n");
//                 strcpy ( message_status.status_message, "COULD NOT OPEN FILE");
//                 return message_status;
//             }
//             else{
//                 printf("File opened successfully\n");
//                 strcpy ( message_status.status_message, "OPENED SUCCESSFULLY");

//                 //return message_status;

//             }


//             // Read the file contents.


//             // Packet file_content[MAX_LENGTH];
//             // char file_data2[MAX_LENGTH];
//             // int temp_count = 0;


//             // while (fgets(file_data2, MAX_LENGTH, file_pointer) != NULL) {
//             //     strcpy(file_content[temp_count++].data, file_data2);
//             // }

//             // file_content[0].total_chunks = temp_count;

//             // // Close the file.
//             // fclose(file_pointer);
//             // printf("File Closed successfully\n");


//             //memcpy(message_status.file_or_folder_content, file_content, MAX_LENGTH * MAX_LENGTH);

//             //return message_status;
//             fseek(file_pointer, 0L, SEEK_END); 
//             long long int length_of_the_file= ftell(file_pointer); 
//             fseek(file_pointer, 0, SEEK_SET); // Reset file pointer to the beginning.

//     // char *file_content = (char *)malloc(length_of_the_file + 1); // +1 for null terminator.
//     // if (file_content == NULL) {
//     //     printf("Memory allocation error.\n");
//     //     strcpy(message_status.status_message, "MEMORY ALLOCATION ERROR");
//     //     fclose(file_pointer);
//     //     return message_status;
//     // }

//     // size_t read_size = fread(file_content, 1, length_of_the_file, file_pointer);
//     // if (read_size != length_of_the_file) {
//     //     printf("Error reading the file.\n");
//     //     strcpy(message_status.status_message, "ERROR READING FILE");
//     //     free(file_content);
//     //     fclose(file_pointer);
//     //     return message_status;
//     // }

//     // // Null-terminate the content to treat it as a string.
//     // file_content[length_of_the_file] = '\0';

//     // printf("Total size of the file: %lld\n", length_of_the_file);
//     // printf("File data:\n%s", file_content);

//     // fclose(file_pointer);
//     // free(file_content);
//     // printf("File read and printed successfully\n");
//     //fseek(file_pointer, 0, SEEK_SET); // Reset file pointer to the beginning.

//     int chunk_size = sizeof(Packet) - sizeof(int); // Calculate the size of data portion in Packet.
//     int total_chunks = (length_of_the_file + chunk_size - 1) / chunk_size; // Calculate total chunks.

//     Packet *file_content = (Packet *)malloc(total_chunks * sizeof(Packet));
//     if (file_content == NULL) {
//         printf("Memory allocation error.\n");
//         strcpy(message_status.status_message, "MEMORY ALLOCATION ERROR");
//         fclose(file_pointer);
//         return message_status;
//     }

//     for (int i = 0; i < total_chunks; i++) {
//         file_content[i].total_chunks = total_chunks;
//         int bytes_read = fread(file_content[i].data, 1, chunk_size, file_pointer);
        
//         if (bytes_read < 0) {
//             printf("Error reading the file.\n");
//             strcpy(message_status.status_message, "ERROR READING FILE");
//             free(file_content);
//             fclose(file_pointer);
//             return message_status;
//         }
//     }

//     fclose(file_pointer);

//     printf("Total size of the file: %lld\n", length_of_the_file);

//     // You can now access the file content in chunks using file_content array.
//     for (int i = 0; i < total_chunks; i++) {
//         printf("%s", file_content[i].data);
//     }

//     printf("File read successfully in %d chunks\n", total_chunks);

//     // Don't forget to free the allocated memory when you're done with it.
//     free(file_content);
//     return message_status;
// }
#include "../headers.h"

acknowledgmentMessage reading_the_file(char file_name[MAX_LENGTH], acknowledgmentMessage message_status){

          //read a file 
            // Open the file.
            printf("%s\n", file_name);
            FILE *file_pointer= fopen("file_read.c", "r");

            // Check if the file opened successfully.
            if (file_pointer == NULL) {
                printf("Could not open file.\n");
                strcpy ( message_status.status_message, "COULD NOT OPEN FILE");
                return message_status;
            }
            else{
                printf("File opened successfully\n");
                strcpy ( message_status.status_message, "OPENED SUCCESSFULLY");
                return message_status;
            }

            // Read the file contents.
            fseek(file_pointer, 0L, SEEK_END); 
            long long int length_of_the_file= ftell(file_pointer); 
            printf("Total size of the file: %lld\n", length_of_the_file);

            Packet file_content[length_of_the_file];
            char file_data[MAX_LENGTH];
            int temp_count = 0;

            printf("File data:\n");
            while (fgets(file_data, sizeof(file_data), file_pointer) != NULL) {
                // file_content[temp_count].total_chunks = length_of_the_file;
                // strcpy(file_content[temp_count++].data, file_data);
                printf("%s", file_data);
            }

            // Close the file.
            fclose(file_pointer);
            printf("File read successfully\n");

            for ( int i = 0; i < length_of_the_file; i++ ) { 
                printf("%s ", file_content[i].data);
            }
        
}
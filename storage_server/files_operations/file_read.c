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
                strcpy(message_status.file_or_folder_content[0].data, file_data2);
            }

            // file_content[0].total_chunks = temp_count;

            // Close the file.
            fclose(file_pointer);
            printf("File Closed successfully\n");


            // memcpy(message_status.file_or_folder_content, file_content, MAX_LENGTH * MAX_LENGTH);

            return message_status;
}

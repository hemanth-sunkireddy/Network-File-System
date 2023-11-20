#include "../headers.h"


acknowledgmentMessage copying_the_file(char final_path_of_the_source[MAX_LENGTH_OF_PATH], Copy_source_dest details_of_source_destination_storage_server, acknowledgmentMessage message_status, int destination_ss_port_no)
{
      if ( details_of_source_destination_storage_server.source_or_destination_storage_server == 0 ) { 
          // Function for soruce storage server functionality. 
           FILE *file_pointer= fopen(final_path_of_the_source, "r");

            char file_data2[MAX_LENGTH];

            while (fgets(file_data2, MAX_LENGTH, file_pointer) != NULL) {
                strcpy(message_status.status_message, file_data2);
                printf("Content of the file: %s\n", message_status.status_message);
            }

            // Close the file.
            fclose(file_pointer);
            printf("File copying successful and sent to the naming server.\n");
            return message_status;
      }
      else{
          // Function for destination storage server functionality. 
          printf("Source file: %s\n", details_of_source_destination_storage_server.source_path);
          printf("Destination path: %s\n", details_of_source_destination_storage_server.destination_path);
          printf("Content of the file: %s\n", details_of_source_destination_storage_server.file_content);
          int final_port_no = destination_ss_port_no - 4999; 
          printf("Final destination port Number: %d\n", final_port_no);
          
          // SS string. 
          char ss[MAX_LENGTH * 4];
          strcpy(ss, "SS");
          char destination_ss_no_in_char[5];
          sprintf(destination_ss_no_in_char, "%d", final_port_no);
          strcat(ss, destination_ss_no_in_char);
          char slash_character[2];
          strcpy(slash_character, "/");
          strcat(ss, slash_character);
          strcat(ss, details_of_source_destination_storage_server.destination_path);
          strcat(ss, slash_character);
          strcat(ss, details_of_source_destination_storage_server.source_path);
        
            printf("Final path to copy the file: %s\n", ss);
          // Create a file and append the data Now. 
           // Write the string to a file
            FILE *file = fopen(ss, "w");
            if (file == NULL) {
                perror("Error opening file");
                strcpy(message_status.status_message, "ERROR OPENING FILE");
                return message_status;
            }

            // Write the input string with newline character.
            fprintf(file, "%s\n", details_of_source_destination_storage_server.file_content);

            // Close the file and free the input buffer.
            fclose(file);

      }
      strcpy(message_status.status_message, "Copying a file successfully.");
      return message_status;
}

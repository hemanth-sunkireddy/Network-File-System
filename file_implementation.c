#include <stdio.h> 
#include <stdlib.h> 
#define MAX_LENGTH 1024
int main() { 

      // Declaring the variable to store the file contents to the string. 
      char file_content[MAX_LENGTH];


      // Opening file pointer 
      FILE* file_pointer;
      file_pointer = fopen("client.c", "r");
      
      // Error checking for file pointer 
      if ( file_pointer == NULL ){
	printf("Error while opening the file");
      }	
      else{
	      printf("File opened successfully\n");
      }
      
      // Reading operation from file 
      int reading_status ;
      reading_status = fread(file_content, MAX_LENGTH, 1, file_pointer); 
      printf("%s\n", file_content);
      



      // Closing the file pointer
      fclose ( file_pointer);
      



	return 0;
}


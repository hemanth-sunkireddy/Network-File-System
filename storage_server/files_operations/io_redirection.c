


void redirect_output_in_file(char output_file_name[250],int append_mode) {
    // Determine the file open mode based on the append_mode parameter
    int open_mode;
    if (append_mode) {
        open_mode = O_WRONLY | O_CREAT | O_APPEND; // Append mode
    } else {
        open_mode = O_WRONLY | O_CREAT | O_TRUNC;  // Overwrite mode
    }

    // Open the output file
    int output_fd = open(output_file_name, open_mode, 0644);
    
    
    if (output_fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // Redirect standard output (stdout) to the output file descriptor
    if (dup2(output_fd, STDOUT_FILENO) == -1) {
        perror("dup2");
        exit(EXIT_FAILURE);
    }

    // Close the file descriptor for the output file (stdout is still redirected)
    close(output_fd);
}


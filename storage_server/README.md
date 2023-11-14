# Storage server Documentation  

### How to run 
1. Run make in storage_server directory. 
2. Run ./storage_server 

### Initialisation of storage servers.
1. Taking first storage number as 5000. And then each storage server incremented by 1. ( 5000, 5001, 5002,...)
2. For now, IP address is same for all. (127.0.0.1) - implementing different ip's now. 
3. For Now, Fixed storage servers count is 4. (Initialised 4 storage servers.)
4. And I am creating four folders for 4 storage servers in the root directory of storage_server to seperate them easily. 
5. Added basic code of initialisation of all storage servers. 
6. Need to add main storage server to add new storage server dynamically. 

### Implementation of recieving requests. 
1. Working on only one naming server, one client for now. ( Multiple clients will implement later.)
2. Initially, Client connects with particular socket of storage server. The request contains fileNameAndOperation struct. ( i.e; operation_number, Path, Storage    server number, integer value for whether naming server or client connected).
3. naming_server_or_client = 0 represents client connected. else, naming_server_or_client = 1 represents naming server connected in the fileNameAndOperation struct. 
4. Later going to the respective storage server and perform the operation mentioned.
5. How to find naming server/ client connected using operation number he is sending. If operation number == 3 || 4 || 8 || 9 then naming server connected, else client connnected.
### Individual Operations 
#### 1. Reading a File
1. Client will choose operation 1. 
2. Client will give file Name. 
3. Assuming the default folder / path where file opening happens is in the storage_server folder. 
4. Copying the data in the Packets structs with each packet sub array contains 1024 character length. 
5. Copying the total number of chunks in message_status.file_or_folder_content[0].total_chunks.
6. And the data in the respective sub chunks of message_status.file_or_folder_content[i].data.

#### 2. Writing a File 
1. Need to implement.

#### 3. Deleting a File 
1. Naming server asks to delete a file by giving the path.
2. If path is not correct and permissions not satisfied, error message sent. 
3. Else File deletes and success message sent to naming server.

#### 4. Creating a File
1. Naming server give the file name along with path relative to the Server Folder. 
2. storage server Checks for permission of the parent folder and creates a new file if not exists. May be create a new file even already exists. 
3. May implement some cases where if file already exists it will show warning or error. But not for now. 

#### 5. Getting additional information of File 
1. Client give the file name. 
2. Sending the additional information of File by using fileInformation struct along with acknowledgment message to the client.

#### 6. Getting Folder information
1. Client gives the folder path. (Assuming staring point is Server folder)
2. If exists then fetch number of files, number of folders, total size of the folder, permissions, last modified time and created time. 
3. Send the data to  the client. 
4. (Need to document here properly later.) Only folder name sent to client for now. Working on sending all details to client. 

#### 7. Listing all Files and Folders
1. Client asks for folder information. 
2. Sending all the files and folders using struct list_of_all_files_folders for sending the all the files and folders to the client along with status message. 

#### 8. Deleting a Folder
1. Naming server gives the path respective to the storage_server Folder. 
2. Deleting Folder when no SubFolder present implemented. Deletion of SubFolder also implemented.
3. Deleting a Folder which has subFolders. ( pending, working on it).
4. Checked directory existence before performing folder deletion. (Done).
5. Deleting a Folder which have write permission but subFolder dont have the write permission. (Edge Case may implement later)

#### 9. Creating a Folder 
1. All new folders will be created in the Server directory. 
2. Implemented one Folder creation in the Server directory.
3. If folder already exists, error in creating folder.
4. To create subfolder, first will check for parent folder existence. If parent folder exist then success in creating folder, else error in creating folder.
5. While creating the folder, Permissions of the parent folder checking. If parent folder has no written permission, then error.
6. Also implemented other errors. (Need to document here properly later.)



### Handling multiple clients to different storage servers. 
1. May use select() to solve this issue. 

### Handling multiple clients to same storage server. 
1. May be select() function or multi_threading implementation. 

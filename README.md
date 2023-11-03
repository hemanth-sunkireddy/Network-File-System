# OSN COURSE PROJECT 

## How to run 
1. Clone the repository and cd into the project.
2. Run the below command in terminal 

```
make
```
3. Go to the Server Folder and  run the below command to activate server. 
```
./server
```
4. Open another terminal and go to the Client folder then run the below command to activate client.
```
./client
```

## Documentation 

### Folder structure
```
.
├── Client
│   ├── client
│   ├── client.c
│   ├── client_receivingResponse.c
│   └── client_sendingRequest.c
├── files_operations
│   ├── file_create.c
│   ├── file_delete.c
│   ├── file_information.c
│   ├── file_read.c
│   ├── file_write.c
│   └── io_redirection.c
├── folder_operations
│   ├── folder_create.c
│   ├── folder_delete.c
│   ├── folder_information.c
│   ├── folder_read.c
│   └── folder_write.c
├── headers.h
├── Makefile
├── README.md
└── Server
    ├── server
    ├── server.c
    ├── server_receivingRequest.c
    └── server_sendingResponse.c

4 directories, 22 files
```

### Operations 
```
1 - Reading a File
2 - Writing a File
3 - Deleting a File
4 - Creating a File
5 - Getting Additional information of File
6 - Getting Additional information of Folder
7 - Writing a Folder
8 - Deleting a Folder
9 - Creating a Folder
10 - Listing all files and Folders
```

### Basic work flow
1. Client will choose operation Number. 
2. Client will choose file / folder name. 
3. Server process the request. 
4. Server Sends the acknowledgement Message. 
5. Along with acknowledgment message, if client asks for reading, writing or listing then respective data also sent to client. 


### Individual Operations 
#### 1. Reading a File
1. Client will choose operation 1. 
2. Client will give file Name. 
3. Assuming the default folder / path where file opening happens is in the server folder. 
4. Copying the data in the Packets structs with each packet sub array contains 1024 character length. 
5. Copying the total number of chunks in message_status.file_or_folder_content[0].total_chunks.
6. And the data in the respective sub chunks of message_status.file_or_folder_content[i].data.



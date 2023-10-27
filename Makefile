all: 
	gcc Server/server.c Server/server_sendingResponse.c Server/server_receivingRequest.c files_operations/file_read.c files_operations/file_write.c files_operations/file_delete.c files_operations/file_information.c files_operations/file_create.c folder_create.c folder_delete.c folder_information.c folder_read.c folder_write.c list_filesFolders.c -o Server/server
	gcc Client/client.c Client/client_sendingRequest.c Client/client_receivingResponse.c -o Client/client


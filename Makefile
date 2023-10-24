all: 
	gcc server.c server_sendingResponse.c server_receivingRequest.c file_read.c file_write.c file_delete.c file_information.c file_create.c folder_create.c folder_delete.c folder_information.c folder_read.c folder_write.c list_filesFolders.c -o server
	gcc client.c client_sendingRequest.c client_receivingResponse.c -o client


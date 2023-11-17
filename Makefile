all: 
	gcc client/client.c client/client_receving_data.c  -o client/client_run
	gcc naming_server/nm.c naming_server/nm_receivingRequest.c  -o naming_server/naming_server_run
	gcc storage_server/storage_server.c storage_server/storage_server_recieving_request.c storage_server/storage_server_sending_response.c  storage_server/files_operations/file_delete.c storage_server/files_operations/file_write.c storage_server/files_operations/file_read.c storage_server/files_operations/file_information.c   storage_server/folder_operations/folder_delete.c storage_server/folder_operations/folder_read.c storage_server/folder_operations/folder_information.c storage_server/folder_operations/folder_create.c storage_server/files_operations/file_create.c -o storage_server/storage_server_run



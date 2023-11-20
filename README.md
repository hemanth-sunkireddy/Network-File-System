# OSN COURSE PROJECT 


## How to Run 
1. Run make in the final_project_73 root directory.
2. For 1st storage activation, Go through stroage_server folder and run ./run_storage_server1
3. For 2nd storage activation, Go through stroage_server folder and run ./run_storage_server2
4. For 1st storage activation, Go through stroage_server folder and run ./run_storage_server3
5. For 1st storage activation, Go through stroage_server folder and run ./run_storage_server4
6. For 1st storage activation, Go through stroage_server folder and run ./run_storage_server5
7. For naming server activation, Go through naming_server folder and run ./naming_server_run 
8. For client activation, Go through client folder and run ./client_run 

### Operations 

```
1 - Reading a File
2 - Writing a File
3 - Deleting a File
4 - Creating a File
5 - Getting Additional information of File 
6 - Getting Additional information of Folder
7 - Listing all Files and Folders
8 - Deleting a Folder
9 - Creating a Folder
10- Copy Folder
11- Copy File
``` 

### Testing: 
#### Copy File Operation
1. Client will give source file path and destination file path - True
2. Naming server will check for the storage server source path and storage server destination path - True
3. Naming server Connects to the source storage server and sends all the necessary details of the destination storage server - True
4. Source storage server recieves the details of the file contents to the naming server - True
5. Naming server connects to the destination path storage server - True
6. Naming sends the file name and file content to the destination path storage server - True
7. Destination storage server recieves the file name and file content and adds it to the respective path - True 
8. Destination storage server sends the acknowledgment status to the Naming server. - True 
9. Then source storage server sends this status back to the client - True

#### Copy Folder operation 
1. Client will give source folder path and destination folder path - False
2. Naming server will check for the storage server source path and storage server destination path - False
3. Naming server Connects to the source storage server and sends all the necessary details of the destination storage server - False 
4. Source storage server recieves the details of the destination storage server - False 
5. Source storage server connects to the destination path storage server - False
6. Source storage server sends the folder name and folder content to the destination path storage server - False 
7. Destination storage server recieves the file name and file content and adds it to the respective path - False 
8. Destination storage server sends the acknowledgment status to the source storage server. - False 
9. Then source storage server sends this status back to the client - False

### Testing multiple clients to the naming server. 
// Need to implement. 

 ### LRU CACHE IMPLEMENTATION: 
// NEED TO IMPLEMENT. 

### BOOKKEEPING: 
// NEED TO IMPLEMENT. 

### ERROR CODES: 
// NEED TO IMPLEMENT. 

### REDUNDANCY: 
// NEED TO IMPLEMENT. 

### Assumptions: 
1. If two different storage servers have same file name at the root level. ( NOT EXISTENCE CASE). 


### Storage Server
1. In README file of storage_server folder. 

### Naming Server 
1. In README file of naming_server folder. 

### client 
1. In README file of client folder.


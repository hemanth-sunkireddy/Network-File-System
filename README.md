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
Testing the code for single client of all operations. 
#### Read File Operation
1. Client sending operation number and path - False 
2. Naming server receiving correct operation no and path - False 
3. Naming server performing necessary operation - False 
4. Naming server correct sending storage server info to client - False 
5. Client connecting to the given storage server - False 
6. Client receiving the content of the File and status message - False 

#### Write File Operation
1. Client sending operation number and path - False 
2. Naming server receiving correct operation number and path - False 
3. Naming server performing necessary operation - False 
4. Naming server correct sending storage server info to client - False 
5. Client connecting to the given storage server - False 
6. Client sending data to write in file and storage server receiving data - False
7. Client receiving the status message - False 

#### Delete File operation
1. Client sending operation number and path - True
2. Naming server receiving correct operation number and path - True 
3. Naming server performing necessary operation - True
4. Naming server gets correct storage server to connect - True
5. Naming server connecting to the given storage server - True 
6. Storage server performing operation successful - True
7. Naming server receiving the status message from storage server - True 
8. Naming server sending back the status message to client - True

#### Create File operation
1. Client sending operation number and path - True
2. Naming server receiving correct operation number and path - True
3. Naming server performing necessary operation - True 
4. Naming server gets correct storage server to connect - True
5. Naming server connecting to the given storage server - True
6. Storage server performing operation successful - True
7. Naming server receiving the status message from storage server - True 
8. Naming server sending back the status message to client - True

#### File Information operation
1. Client sending operation number and path - False 
2. Naming server receiving correct operation number and path - False 
3. Naming server performing necessary operation - False 
4. Naming server correct sending storage server info to client - False 
5. Client connecting to the given storage server - False 
6. Storage server sending file information success - False
7. Client receiving the status message and file information - False 

#### Folder information operation 
1. Client sending operation number and path - False 
2. Naming server receiving correct operation number and path - False 
3. Naming server performing necessary operation - False 
4. Naming server correct sending storage server info to client - False 
5. Client connecting to the given storage server - False 
6. Storage server sending folder information success - False
7. Client receiving the status message and folder information - False

#### Listing all Files information 
1. Client sending operation number and path - False 
2. Naming server receiving correct operation number and path - False 
3. Naming server performing necessary operation - False 
4. Naming server correct sending storage server info to client - False 
5. Client connecting to the given storage server - False 
6. Storage server sending list of all files and folders success - False
7. Client receiving the status message and files and folders list - False

#### Delete Folder Operation 
1. Client sending operation number and path - False 
2. Naming server receiving correct operation number and path - False 
3. Naming server performing necessary operation - False 
4. Naming server gets correct storage server to connect - False
5. Naming server connecting to the given storage server - False 
6. Storage server performing operation successful - False
7. Naming server receiving the status message from storage server - False 
8. Naming server sending back the status message to client - False

#### Create Folder operation 
1. Client sending operation number and path - True 
2. Naming server receiving correct operation number and path - True 
3. Naming server performing necessary operation - True 
4. Naming server gets correct storage server to connect - True
5. Naming server connecting to the given storage server - True 
6. Storage server performing operation successful - True
7. Naming server receiving the status message from storage server - True 
8. Naming server sending back the status message to client - True

#### Copy Folder operation 
// Need to implement.

#### Copy File operation 
// Need to implement. 

#### Creating New storage server (Dynamically)
// Need to implement. 


### Testing multiple clients to the naming server. 
// Need to implement. 

### Testing multiple clients to the storage server. 
// Need to implement. 

### Concurrent file Reading. 
// Need to implement. 

### Assumptions: 
1. If two different storage servers have same file name at the root level. ( NOT EXISTENCE CASE). 


### Storage Server
1. In README file of storage_server folder. 

### Naming Server 
1. In README file of naming_server folder. 

### client 
1. In README file of client folder.


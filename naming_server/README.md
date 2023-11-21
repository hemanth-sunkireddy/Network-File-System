# Naming server Documentation

**How to run**
    1. After running make command in the final-project-73 directory, change to naming_server directory
    2. Run ./naming_server_run

Initialisation:
    The port is initialised to 5000 and the ip to be 127.0.0.1.
    Maximum number of storage server are initialised with port numbers equal to (5000+storage server number) and the ip being the same.
    Initially, the client connects to the naming server and sends the operation number which the client needs to be implemented.

Operations implementation
    (For the operation numbers list, refer to the client's code)
    Create and delete operations:
        If the operation number lies in the set {3,4,8,9}, the naming server finds which storage server is not full and creates the file there if it is a single file or folder by connecting to the respective storage server and stored in the accessible paths.

        Otherwise, it searches in the accessible paths to find the storage server in which it has to be created/deleted and creates/deletes the file by connecting to the respective storage server.

    Read, write, getting additional information and listing of all files and folders:
        If the operation number lies in the set {1,2,5,6,7}, it searches in the accessible paths and finds the storage server number, port and ip to send the details of this storage server and the file/folder name along with operation to the client.

    Copying file or folder:
        If the operation number is either 10 or 11, the source and destination paths location(storage servers in which they are present) is found and then by connecting to them, the functionality of copying is done.


For implementing multiple clients, select is used.

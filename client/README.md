# Client side Documentation

Client first connects with NS.
1)Naming server can perform operations and send status message
2)Return SS ip,port number 
  Client connects using port,ip to SS(can be multiple for Redundancy) and perform required task




Ignore for redundancy
//Reading a File\n");
// printf("5 - Getting Additional information of File \n");         naming server finds single server(out of redundant's) contiang file and lists  for it done 
// printf("6 - Getting Additional information of Folder\n");          naming server finds single server(out of redundant's) contiang folder and lists  for it done 
// printf("7 - Listing all Files and Folders\n");                    ??


  
Need of Redundancy
  
printf("2 - Writing a File\n");
printf("3 - Deleting a File\n");
printf("4 - Creating a File\n");
printf("8 - Deleting a Folder\n");
printf("9 - Creating a Folder\n");
printf("10- Copy Folder\n");
printf("11- Copy File\n");


Whenever there's need to make storage server-> simultaneously make +1 server needed for redundancy ->simple
writing file -> write in both location of file simultaneously
deleting file-> delete in both location of file simultaneously
creting file ->
deleting folder->
creting folder-> -- in both location of file simultaneously

Copy Folder  -> in location of server  ; as already we may have 2 diff server's , why to create copy of copy?  ->if write requested ->we'll need to write in this copy ;see Approch Below 
copy file    -> in location of server  ; as already we may have 2 diff server's , why to create copy of copy?

Approach: For Writing -> make function that lists all path of file.txt, then write at all loactions of file **  
writing may be long code() so we'll simply copy file.txt to listed locations

// Path handler code 

// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <stdbool.h>

// #define MAX_PATH_LENGTH 256

// // printFullPathToNode  ,isPathValid(  are functions for implementing trie
// // printFullPathToNode : prints complete path from root node till given node , so you send delete file1.txt -> gives full path for doing operations
// //ispAthValid()   ->  check   if relative path or absolute path


// struct PathNode {
//     char name[MAX_PATH_LENGTH];
//     struct PathNode* parent;
//     struct PathNode* children;
// };

// struct PathNode* createPathNode(const char* name) {
//     struct PathNode* node = (struct PathNode*)malloc(sizeof(struct PathNode));
//     if (node) {
//         strncpy(node->name, name, MAX_PATH_LENGTH);
//         node->parent = NULL;
//         node->children = NULL;
//     }
//     return node;
// }

// void addChild(struct PathNode* parent, struct PathNode* child) {
//     child->parent = parent;
//     child->children = NULL;
//     if (parent->children == NULL) {
//         parent->children = child;
//     } else {
//         struct PathNode* current = parent->children;
//         while (current->children != NULL) {
//             current = current->children;
//         }
//         current->children = child;
//     }
// }

// char* getFullPath(struct PathNode* node) {
//     char* path = (char*)malloc(MAX_PATH_LENGTH);
//     path[0] = '\0';

//     while (node != NULL) {
//         char temp[MAX_PATH_LENGTH];
//         strcpy(temp, path);
//         strcpy(path, "/");
//         strcat(path, node->name);
//         strcat(path, temp);
//         node = node->parent;
//     }

//     return path;
// }
// bool isPathValid(struct PathNode* root, const char* path) {
//     struct PathNode* currentNode = root;
//     char tempPath[MAX_PATH_LENGTH];
//     strncpy(tempPath, path, MAX_PATH_LENGTH);
//     char* token = strtok(tempPath, "/");

//     while (token != NULL) {
//         bool found = false;
//         struct PathNode* child = currentNode->children;

//         while (child != NULL) {
//             if (strcmp(child->name, token) == 0) {
//                 currentNode = child;
//                 found = true;
//                 break;
//             }
//             child = child->children;
//         }

//         if (!found) {
//             return false; // The path is invalid
//         }

//         token = strtok(NULL, "/");
//     }

//     return true; // The path is valid
// }
// void printFullPathToNode(struct PathNode* root, const char* targetName) {
//     struct PathNode* target = root->children;

//     while (target != NULL) {
//         if (strcmp(target->name, targetName) == 0) {
//             char* fullPath = getFullPath(target);
//             printf("Full Path: %s\n", fullPath);
//             free(fullPath);
//             return;
//         }
//         target = target->children;
//     }

//     printf("Path not found\n");
// }

// int main() {
//     struct PathNode* root = createPathNode("root");
//     struct PathNode* folder1 = createPathNode("folder1");
//     struct PathNode* folder2 = createPathNode("folder2");
//     struct PathNode* file1 = createPathNode("file1.txt");
//     struct PathNode* fol4 = createPathNode("fol4");
//     struct PathNode* file2 = createPathNode("file2.txt");
//     struct PathNode* folder3 = createPathNode("folder3");
//     struct PathNode* file3 = createPathNode("file3.txt");

//     addChild(root, folder1);
//     addChild(root, folder2);
//     addChild(folder1, file1);
//     addChild(folder1, file2);
//     addChild(root, folder3);
//     addChild(folder3, file3);
//     addChild(folder3, fol4);
//     //full path from root
//     printFullPathToNode(root, "fol4");
    
//     // Check if paths are valid
//     const char* path1 = "file1.txt";
//     const char* path2 = "folder2/file2.txt";
//     const char* path3 = "folder3/file3.txt";
//     const char* path4 = "folder1/invalid.txt";

//     printf("Path1 is valid: %d\n", isPathValid(root, path1));
//     printf("Path2 is valid: %d\n", isPathValid(root, path2));
//     printf("Path3 is valid: %d\n", isPathValid(root, path3));
//     printf("Path4 is valid: %d\n", isPathValid(root, path4));

//     // Clean up the memory (free all nodes)
//     free(root);
//     free(folder1);
//     free(folder2);
//     free(file1);
//     free(file2);
//     free(folder3);
//     free(file3);

    
// }










// Trie Code 
//FOR YOUR TESTING
// #include <stdio.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <stdbool.h>

// #define MAX_PATH_LENGTH 256

// struct PathNode {
//     char name[MAX_PATH_LENGTH];
//     struct PathNode* parent;
//     struct PathNode* children;
// };

// struct PathNode* createPathNode(const char* name) {
//     struct PathNode* node = (struct PathNode*)malloc(sizeof(struct PathNode));
//     if (node) {
//         strncpy(node->name, name, MAX_PATH_LENGTH);
//         node->parent = NULL;
//         node->children = NULL;
//     }
//     return node;
// }

// void addChild(struct PathNode* parent, struct PathNode* child) {
//     child->parent = parent;
//     child->children = NULL;
//     if (parent->children == NULL) {
//         parent->children = child;
//     } else {
//         struct PathNode* current = parent->children;
//         while (current->children != NULL) {
//             current = current->children;
//         }
//         current->children = child;
//     }
// }

// char* getFullPath(struct PathNode* node) {
//     char* path = (char*)malloc(MAX_PATH_LENGTH);
//     path[0] = '\0';

//     while (node != NULL) {
//         char temp[MAX_PATH_LENGTH];
//         strcpy(temp, path);
//         strcpy(path, "/");
//         strcat(path, node->name);
//         strcat(path, temp);
//         node = node->parent;
//     }

//     return path;
// }
// bool isPathValid(struct PathNode* root, const char* path) {
//     struct PathNode* currentNode = root;
//     char tempPath[MAX_PATH_LENGTH];
//     strncpy(tempPath, path, MAX_PATH_LENGTH);
//     char* token = strtok(tempPath, "/");

//     while (token != NULL) {
//         bool found = false;
//         struct PathNode* child = currentNode->children;

//         while (child != NULL) {
//             if (strcmp(child->name, token) == 0) {
//                 currentNode = child;
//                 found = true;
//                 break;
//             }
//             child = child->children;
//         }

//         if (!found) {
//             return false; // The path is invalid
//         }

//         token = strtok(NULL, "/");
//     }

//     return true; // The path is valid
// }
// void printFullPathToNode(struct PathNode* root, const char* targetName) {
//     struct PathNode* target = root->children;

//     while (target != NULL) {
//         if (strcmp(target->name, targetName) == 0) {
//             char* fullPath = getFullPath(target);
//             printf("Full Path: %s\n", fullPath);
//             free(fullPath);
//             return;
//         }
//         target = target->children;
//     }

//     printf("Path not found\n");
// }

// int main() {
//     //ALWAYS MAKE A NODE ,WHILE CREATING FILE, FOLDER OF IT;S PATH
//     struct PathNode* rootm = createPathNode("rootm");
//     struct PathNode* server1 = createPathNode("server1");
//     struct PathNode* server2 = createPathNode("server2");
//     struct PathNode* server3 = createPathNode("server3");
//     struct PathNode* server4 = createPathNode("server4");
    
//     struct PathNode* folder1 = createPathNode("folder1");
//     struct PathNode* folder2 = createPathNode("folder2");
//     struct PathNode* file1 = createPathNode("file1.txt");
//     struct PathNode* fol4 = createPathNode("fol4");
//     struct PathNode* file2 = createPathNode("file2.txt");
//     struct PathNode* folder3 = createPathNode("folder3");
//     struct PathNode* file3 = createPathNode("file3.txt");

    
//     addChild(rootm, server1);
//     addChild(rootm, server2);
//     addChild(rootm, server3);
//     addChild(rootm, server4);
//     //HERE WE ARE REFENCING PARENT
    
//     addChild(server1, folder1);
//     addChild(server2, folder2);
//     addChild(server1, file1);
//     addChild(folder1, file2);
//     addChild(folder2, folder3);
//     addChild(folder3, file3);
//     addChild(folder3, fol4);
//     //full path from rootM
//     printFullPathToNode(rootm, "folder3");

//     // Check if paths are valid
//     const char* path1 = "file1.txt";
//     const char* path2 = "root/folder2/file2.txt";
//     const char* path3 = "folder3/file3.txt";
//     const char* path4 = "folder1/invalid.txt";


//     //IF YOU WANT TO CHECK IF PATH IS VALID -EXTRA FEATURE
//     printf("Path1 is valid: %d\n", isPathValid(rootm, path1));
//     printf("Path2 is valid: %d\n", isPathValid(rootm, path2));
//     printf("Path3 is valid: %d\n", isPathValid(rootm, path3));
//     printf("Path4 is valid: %d\n", isPathValid(rootm, path4));
    

//   //GET FULL PATH FROM FROM ROOT DIRECTORY
//     printFullPathToNode(rootm, "server1");
//     printFullPathToNode(rootm, "server2");
//     printFullPathToNode(rootm, "server3");
//     printFullPathToNode(rootm, "server4");
//     printFullPathToNode(rootm, "folder1");
//     printFullPathToNode(rootm, "folder2");
//     printFullPathToNode(rootm, "file1.txt");
//     printFullPathToNode(rootm, "file2.txt");
//     printFullPathToNode(rootm, "file3.txt");
//     printFullPathToNode(rootm, "folder3");
//     printFullPathToNode(rootm, "fol4");

//     // REAL CASE :
//     // WE NEED TO USE MAP WHICH STORES= CHAR NAME:pointer to struct node  =>
  
//     // INTERT file.txt at  "A/B/C/D"   => 
//     //  1) make node for file =>createPathNode("file.txt");
//     //  2) extract last folder from  "A/B/C/D" =>  "D"  =>  THERE MUST EXIST STRUCT WITH NAME "D" -> FIND ITS ADDRESS MAP<>
//     //  3) addChild( FIND("D"), file);


// }

// // Output :
// // Full Path: /rootm/server2/folder2/folder3
// // Path1 is valid: 1
// // Path2 is valid: 0
// // Path3 is valid: 1
// // Path4 is valid: 0
// // Full Path: /rootm/server1
// // Full Path: /rootm/server2
// // Full Path: /rootm/server3
// // Full Path: /rootm/server4
// // Full Path: /rootm/server1/folder1
// // Full Path: /rootm/server2/folder2
// // Full Path: /rootm/server1/file1.txt
// // Full Path: /rootm/server1/folder1/file2.txt
// // Full Path: /rootm/server2/folder2/folder3/file3.txt
// // Full Path: /rootm/server2/folder2/folder3
// // Full Path: /rootm/server2/folder2/folder3/fol4






// LRU CODE 
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>

// #define HASH_SIZE 10001
// #define MAX_LENGTH 100

// typedef struct FileNameAndOperation {
//     int operation_number;
//     char name_of_file_or_folder[MAX_LENGTH];
// } FileNameAndOperation;

// typedef struct NS_reply {
//     int port;
//     char IP[1024];
//     int SS_no;
// } NS_reply;

// typedef struct FileData {
//     NS_reply ns_reply_data;
// } FileData;

// typedef struct QNode {
//     struct QNode *prev;
//     struct QNode *next;
//     FileNameAndOperation key;
//     FileData data;
// } QNode;

// typedef struct Queue {
//     QNode *head;
//     QNode *tail;
//     int count;
// } Queue;

// typedef struct {
//     int capacity;
//     Queue *queue;
//     QNode **hash;
// } LRUCache;

// QNode *createNewNode(FileNameAndOperation key, FileData data) {
//     QNode *temp = (QNode *)malloc(sizeof(QNode));
//     temp->prev = NULL;
//     temp->next = NULL;
//     temp->key = key;
//     temp->data = data;
//     return temp;
// }

// Queue *createQueue(int numberOfFrames) {
//     Queue *queue = (Queue *)malloc(sizeof(Queue));
//     queue->head = createNewNode((FileNameAndOperation){-1, ""}, (FileData){{0}});
//     queue->tail = createNewNode((FileNameAndOperation){-1, ""}, (FileData){{0}});
//     queue->head->next = queue->tail;
//     queue->tail->prev = queue->head;
//     queue->count = 0;
//     return queue;
// }

// QNode **createHash(int size) {
//     QNode **hash = (QNode **)malloc(size * sizeof(QNode *));
//     memset(hash, 0, size * sizeof(QNode *));
//     return hash;
// }

// void removeNode(LRUCache *cache, QNode *node) {
//     QNode *prev = node->prev;
//     QNode *next = node->next;
//     prev->next = next;
//     next->prev = prev;
// }

// void addNode(LRUCache *cache, QNode *node) {
//     node->prev = cache->queue->head;
//     node->next = cache->queue->head->next;
//     cache->queue->head->next->prev = node;
//     cache->queue->head->next = node;
// }

// void moveToHead(LRUCache *cache, QNode *node) {
//     removeNode(cache, node);
//     addNode(cache, node);
// }

// QNode *popTail(LRUCache *cache) {
//     QNode *node = cache->queue->tail->prev;
//     removeNode(cache, node);
//     return node;
// }

// LRUCache *lRUCacheCreate(int capacity) {
//     LRUCache *lru = (LRUCache *)malloc(sizeof(LRUCache));
//     lru->hash = createHash(HASH_SIZE);
//     lru->queue = createQueue(capacity);
//     lru->capacity = capacity;
//     return lru;
// }

// FileData lRUCacheGet(LRUCache *obj, FileNameAndOperation key) {
//     QNode *current = obj->queue->head->next;
//     while (current != obj->queue->tail) {
//         if (current->key.operation_number == key.operation_number &&
//             strcmp(current->key.name_of_file_or_folder, key.name_of_file_or_folder) == 0) {
//             // Key found, move to the head and return the data
//             moveToHead(obj, current);
//             return current->data;
//         }
//         current = current->next;
//     }
    
//     // Key not found, return a default value or handle accordingly
//     return (FileData){{0}};
// }


// void lRUCachePut(LRUCache *obj, FileNameAndOperation key, FileData value) {
//     QNode *node = obj->hash[key.operation_number];
//     if (!node) {
//         node = createNewNode(key, value);
//         obj->hash[key.operation_number] = node;
//         addNode(obj, node);
//         obj->queue->count++;

//         if (obj->queue->count > obj->capacity) {
//             QNode *tail = popTail(obj);
//             obj->hash[tail->key.operation_number] = NULL;
//             obj->queue->count--;
//             free(tail);
//         }
//     } else {
//         node->data = value;
//         moveToHead(obj, node);
//     }
// }

// void printLRUCache(LRUCache *lru) {
//     QNode *current = lru->queue->head->next;
//     printf("LRUCache: ");
//     while (current != lru->queue->tail) {
//         printf("(%d, %s, %d, %s, %d) ", current->key.operation_number, current->key.name_of_file_or_folder,
//                current->data.ns_reply_data.port, current->data.ns_reply_data.IP,
//                current->data.ns_reply_data.SS_no);

//         current = current->next;
//     }
//     printf("\n");
// }

// void freeLRUCache(LRUCache *obj) {
//     while (obj->queue->head) {
//         QNode *temp = obj->queue->head;
//         obj->queue->head = obj->queue->head->next;
//         free(temp);
//     }
//     free(obj->queue);
//     free(obj->hash);
//     free(obj);
// }

// int main() {
//     int capacity;
//     FileNameAndOperation key;
//     FileData value;

//     printf("Enter the capacity of the LRUCache: ");
//     scanf("%d", &capacity);

//     LRUCache *lruCache = lRUCacheCreate(capacity);

//     while (1) {
//         printf("1. Put\n2. Get\n3. Print\n4. Exit\n");
//         int choice;
//         printf("Enter your choice: ");
//         scanf("%d", &choice);

//         switch (choice) {
//             case 1:
//                 printf("Enter operation number, name of file or folder, port, IP, and SS number to put in LRUCache: ");
//                 scanf("%d %s %d %s %d", &key.operation_number, key.name_of_file_or_folder, &value.ns_reply_data.port,
//                       value.ns_reply_data.IP, &value.ns_reply_data.SS_no);
//                 lRUCachePut(lruCache, key, value);
//                 break;
//             case 2:
//                 printf("Enter operation number and name of file or folder to get from LRUCache: ");
//                 scanf("%d %s", &key.operation_number, key.name_of_file_or_folder);
//                 FileData result = lRUCacheGet(lruCache, key);
//                 printf("Value for key %d: (%d, %s, %d)\n", key.operation_number, result.ns_reply_data.port,
//                        result.ns_reply_data.IP, result.ns_reply_data.SS_no);
//                 break;
//             case 3:
//                 printLRUCache(lruCache);
//                 break;
//             case 4:
//                 freeLRUCache(lruCache);
//                 return 0;
//             default:
//                 printf("Invalid choice. Please try again.\n");
//                 break;
//         }
//     }
// }









// Copy folder to folder and file to file code 
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <dirent.h>
// #include <sys/stat.h>

// #define MAX_PATH_LENGTH 512 // Increase buffer size

// void copyFile(const char *sourcePath, const char *destinationPath) {
//     FILE *sourceFile, *destinationFile;
//     char buffer[1024];
//     size_t bytesRead;

//     sourceFile = fopen(sourcePath, "rb");
//     if (sourceFile == NULL) {
//         perror("Error opening source file");
//         exit(EXIT_FAILURE);
//     }

//     destinationFile = fopen(destinationPath, "wb");
//     if (destinationFile == NULL) {
//         perror("Error opening destination file");
//         fclose(sourceFile);
//         exit(EXIT_FAILURE);
//     }

//     while ((bytesRead = fread(buffer, 1, sizeof(buffer), sourceFile)) > 0) {
//         fwrite(buffer, 1, bytesRead, destinationFile);
//     }

//     fclose(sourceFile);
//     fclose(destinationFile);

//     printf("File copied successfully!\n");
// }

// void copyDirectory(const char *sourcePath, const char *destinationPath) {
//     DIR *sourceDir;
//     struct dirent *entry;

//     sourceDir = opendir(sourcePath);
//     if (sourceDir == NULL) {
//         perror("Error opening source directory");
//         exit(EXIT_FAILURE);
//     }

//     // Create the destination directory if it doesn't exist
//     // Use platform-specific functions for creating directories
//     #ifdef _WIN32
//     _mkdir(destinationPath);
//     #else
//     mkdir(destinationPath, 0755);
//     #endif

//     while ((entry = readdir(sourceDir)) != NULL) {
//         if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
//             char sourceFilePath[MAX_PATH_LENGTH];
//             char destinationFilePath[MAX_PATH_LENGTH];

//             // Increase buffer size for snprintf
//             snprintf(sourceFilePath, sizeof(sourceFilePath), "%s/%s", sourcePath, entry->d_name);
//             snprintf(destinationFilePath, sizeof(destinationFilePath), "%s/%s", destinationPath, entry->d_name);

//             if (entry->d_type == DT_REG) {
//                 copyFile(sourceFilePath, destinationFilePath);
//             } else if (entry->d_type == DT_DIR) {
//                 copyDirectory(sourceFilePath, destinationFilePath);
//             }
//         }
//     }

//     closedir(sourceDir);
//     printf("Directory copied successfully!\n");
// }

// int main() {
    
//     // Use absolute paths for testing
//     //1   Folder Paths
//     char sourcePath[MAX_PATH_LENGTH] = "/home/popos/Cp"; // Change this to a valid source path
//     char destinationPath[MAX_PATH_LENGTH] = "/home/popos/OSN/Concurrency"; // Change this to a valid destination path


//      //2  FILE PATHS: ## mention file.txt in destination path
//     char sourcePath[MAX_PATH_LENGTH] = "/home/popos/sql.txt"; // Change this to a valid source path
//     char destinationPath[MAX_PATH_LENGTH] = "/home/popos/OSN/Concurrency/sql.txt"; // Change this to a valid destination path

    
//     DIR *dir = opendir(sourcePath);
//     if (dir) {
//         closedir(dir);
//         copyDirectory(sourcePath, destinationPath);
//     } else {
//         copyFile(sourcePath, destinationPath);
//     }

//     return 0;
// }


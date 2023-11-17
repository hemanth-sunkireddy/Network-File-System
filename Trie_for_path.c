//FOR YOUR TESTING
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_PATH_LENGTH 256

struct PathNode {
    char name[MAX_PATH_LENGTH];
    struct PathNode* parent;
    struct PathNode* children;
};

struct PathNode* createPathNode(const char* name) {
    struct PathNode* node = (struct PathNode*)malloc(sizeof(struct PathNode));
    if (node) {
        strncpy(node->name, name, MAX_PATH_LENGTH);
        node->parent = NULL;
        node->children = NULL;
    }
    return node;
}

void addChild(struct PathNode* parent, struct PathNode* child) {
    child->parent = parent;
    child->children = NULL;
    if (parent->children == NULL) {
        parent->children = child;
    } else {
        struct PathNode* current = parent->children;
        while (current->children != NULL) {
            current = current->children;
        }
        current->children = child;
    }
}

char* getFullPath(struct PathNode* node) {
    char* path = (char*)malloc(MAX_PATH_LENGTH);
    path[0] = '\0';

    while (node != NULL) {
        char temp[MAX_PATH_LENGTH];
        strcpy(temp, path);
        strcpy(path, "/");
        strcat(path, node->name);
        strcat(path, temp);
        node = node->parent;
    }

    return path;
}
bool isPathValid(struct PathNode* root, const char* path) {
    struct PathNode* currentNode = root;
    char tempPath[MAX_PATH_LENGTH];
    strncpy(tempPath, path, MAX_PATH_LENGTH);
    char* token = strtok(tempPath, "/");

    while (token != NULL) {
        bool found = false;
        struct PathNode* child = currentNode->children;

        while (child != NULL) {
            if (strcmp(child->name, token) == 0) {
                currentNode = child;
                found = true;
                break;
            }
            child = child->children;
        }

        if (!found) {
            return false; // The path is invalid
        }

        token = strtok(NULL, "/");
    }

    return true; // The path is valid
}
void printFullPathToNode(struct PathNode* root, const char* targetName) {
    struct PathNode* target = root->children;

    while (target != NULL) {
        if (strcmp(target->name, targetName) == 0) {
            char* fullPath = getFullPath(target);
            printf("Full Path: %s\n", fullPath);
            free(fullPath);
            return;
        }
        target = target->children;
    }

    printf("Path not found\n");
}

int main() {
    //ALWAYS MAKE A NODE ,WHILE CREATING FILE, FOLDER OF IT;S PATH
    struct PathNode* rootm = createPathNode("rootm");
    struct PathNode* server1 = createPathNode("server1");
    struct PathNode* server2 = createPathNode("server2");
    struct PathNode* server3 = createPathNode("server3");
    struct PathNode* server4 = createPathNode("server4");
    
    struct PathNode* folder1 = createPathNode("folder1");
    struct PathNode* folder2 = createPathNode("folder2");
    struct PathNode* file1 = createPathNode("file1.txt");
    struct PathNode* fol4 = createPathNode("fol4");
    struct PathNode* file2 = createPathNode("file2.txt");
    struct PathNode* folder3 = createPathNode("folder3");
    struct PathNode* file3 = createPathNode("file3.txt");

    
    addChild(rootm, server1);
    addChild(rootm, server2);
    addChild(rootm, server3);
    addChild(rootm, server4);
    //HERE WE ARE REFENCING PARENT
    
    addChild(server1, folder1);
    addChild(server2, folder2);
    addChild(server1, file1);
    addChild(folder1, file2);
    addChild(folder2, folder3);
    addChild(folder3, file3);
    addChild(folder3, fol4);
    //full path from rootM
    printFullPathToNode(rootm, "folder3");

    // Check if paths are valid
    const char* path1 = "file1.txt";
    const char* path2 = "root/folder2/file2.txt";
    const char* path3 = "folder3/file3.txt";
    const char* path4 = "folder1/invalid.txt";


    //IF YOU WANT TO CHECK IF PATH IS VALID -EXTRA FEATURE
    printf("Path1 is valid: %d\n", isPathValid(rootm, path1));
    printf("Path2 is valid: %d\n", isPathValid(rootm, path2));
    printf("Path3 is valid: %d\n", isPathValid(rootm, path3));
    printf("Path4 is valid: %d\n", isPathValid(rootm, path4));
    

  //GET FULL PATH FROM FROM ROOT DIRECTORY
    printFullPathToNode(rootm, "server1");
    printFullPathToNode(rootm, "server2");
    printFullPathToNode(rootm, "server3");
    printFullPathToNode(rootm, "server4");
    printFullPathToNode(rootm, "folder1");
    printFullPathToNode(rootm, "folder2");
    printFullPathToNode(rootm, "file1.txt");
    printFullPathToNode(rootm, "file2.txt");
    printFullPathToNode(rootm, "file3.txt");
    printFullPathToNode(rootm, "folder3");
    printFullPathToNode(rootm, "fol4");

    REAL CASE :
    WE NEED TO USE MAP WHICH STORES= CHAR NAME:pointer to struct node  =>
  
    INTERT file.txt at  "A/B/C/D"   => 
     1) make node for file =>createPathNode("file.txt");
     2) extract last folder from  "A/B/C/D" =>  "D"  =>  THERE MUST EXIST STRUCT WITH NAME "D" -> FIND ITS ADDRESS MAP<>
     3) addChild( FIND("D"), file);


}
Output :
Full Path: /rootm/server2/folder2/folder3
Path1 is valid: 1
Path2 is valid: 0
Path3 is valid: 1
Path4 is valid: 0
Full Path: /rootm/server1
Full Path: /rootm/server2
Full Path: /rootm/server3
Full Path: /rootm/server4
Full Path: /rootm/server1/folder1
Full Path: /rootm/server2/folder2
Full Path: /rootm/server1/file1.txt
Full Path: /rootm/server1/folder1/file2.txt
Full Path: /rootm/server2/folder2/folder3/file3.txt
Full Path: /rootm/server2/folder2/folder3
Full Path: /rootm/server2/folder2/folder3/fol4


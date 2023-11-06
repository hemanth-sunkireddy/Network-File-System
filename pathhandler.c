
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_PATH_LENGTH 256

// printFullPathToNode  ,isPathValid(  are functions for implementing trie
// printFullPathToNode : prints complete path from root node till given node , so you send delete file1.txt -> gives full path for doing operations
//ispAthValid()   ->  check   if relative path or absolute path


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
    struct PathNode* root = createPathNode("root");
    struct PathNode* folder1 = createPathNode("folder1");
    struct PathNode* folder2 = createPathNode("folder2");
    struct PathNode* file1 = createPathNode("file1.txt");
    struct PathNode* fol4 = createPathNode("fol4");
    struct PathNode* file2 = createPathNode("file2.txt");
    struct PathNode* folder3 = createPathNode("folder3");
    struct PathNode* file3 = createPathNode("file3.txt");

    addChild(root, folder1);
    addChild(root, folder2);
    addChild(folder1, file1);
    addChild(folder1, file2);
    addChild(root, folder3);
    addChild(folder3, file3);
    addChild(folder3, fol4);
    //full path from root
    printFullPathToNode(root, "fol4");
    
    // Check if paths are valid
    const char* path1 = "file1.txt";
    const char* path2 = "folder2/file2.txt";
    const char* path3 = "folder3/file3.txt";
    const char* path4 = "folder1/invalid.txt";

    printf("Path1 is valid: %d\n", isPathValid(root, path1));
    printf("Path2 is valid: %d\n", isPathValid(root, path2));
    printf("Path3 is valid: %d\n", isPathValid(root, path3));
    printf("Path4 is valid: %d\n", isPathValid(root, path4));

    // Clean up the memory (free all nodes)
    free(root);
    free(folder1);
    free(folder2);
    free(file1);
    free(file2);
    free(folder3);
    free(file3);

    
}

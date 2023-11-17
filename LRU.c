

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASH_SIZE 10001
#define MAX_LENGTH 100

typedef struct fileNameAndOperation {
    int operation_number;
    char name_of_file_or_folder[MAX_LENGTH];
} FileNameAndOperation;

typedef struct fileData {
    // Define your data structure here
    // For example, you can add fields like file size, permissions, etc.
    // Replace this comment with the actual fields you need.
    int size;
    // Add more fields as needed
} FileData;

typedef struct qnode {
    struct qnode *prev;
    struct qnode *next;
    FileNameAndOperation key;
    FileData data;
} Qnode;

typedef struct queue {
    Qnode *head;
    Qnode *tail;
    int count;
} Queue;

typedef struct {
    int capacity;
    Queue *queue;
    Qnode **hash;
} LRUCache;

Qnode *createnewnode(FileNameAndOperation key, FileData data) {
    Qnode *temp = (Qnode *)malloc(sizeof(Qnode));
    temp->prev = NULL;
    temp->next = NULL;
    temp->key = key;
    temp->data = data;
    return temp;
}

Queue *createqueue(int numberofframes) {
    Queue *queue = (Queue *)malloc(sizeof(Queue));
    queue->head = createnewnode((FileNameAndOperation){-1, ""}, (FileData){0}); // Dummy values for head
    queue->tail = createnewnode((FileNameAndOperation){-1, ""}, (FileData){0}); // Dummy values for tail
    queue->head->next = queue->tail;
    queue->tail->prev = queue->head;
    queue->count = 0;
    return queue;
}

Qnode **createhash(int size) {
    Qnode **hash = (Qnode **)malloc(size * sizeof(Qnode *));
    memset(hash, 0, size * sizeof(Qnode *));
    return hash;
}

void removeNode(LRUCache *cache, Qnode *node) {
    Qnode *prev = node->prev;
    Qnode *next = node->next;
    prev->next = next;
    next->prev = prev;
}

void addNode(LRUCache *cache, Qnode *node) {
    node->prev = cache->queue->head;
    node->next = cache->queue->head->next;
    cache->queue->head->next->prev = node;
    cache->queue->head->next = node;
}

void movetohead(LRUCache *cache, Qnode *node) {
    removeNode(cache, node);
    addNode(cache, node);
}

Qnode *popTail(LRUCache *cache) {
    Qnode *node = cache->queue->tail->prev;
    removeNode(cache, node);
    return node;
}

LRUCache *lRUCacheCreate(int capacity) {
    LRUCache *lru = (LRUCache *)malloc(sizeof(LRUCache));
    lru->hash = createhash(HASH_SIZE);
    lru->queue = createqueue(capacity);
    lru->capacity = capacity;
    return lru;
}

FileData lRUCacheGet(LRUCache *obj, FileNameAndOperation key) {
    if (obj->hash[key.operation_number] != NULL) {
        Qnode *node = obj->hash[key.operation_number];
        FileData data = node->data;
        movetohead(obj, node);
        return data;
    }
    // You may want to return a default value or handle a case where the key is not found.
    // For simplicity, returning an empty FileData.
    return (FileData){0};
}

void lRUCachePut(LRUCache *obj, FileNameAndOperation key, FileData value) {
    Qnode *node = obj->hash[key.operation_number];
    if (!node) {
        node = createnewnode(key, value);
        obj->hash[key.operation_number] = node;
        addNode(obj, node);
        obj->queue->count++;

        if (obj->queue->count > obj->capacity) {
            Qnode *tail = popTail(obj);
            obj->hash[tail->key.operation_number] = NULL;
            obj->queue->count--;
            free(tail);
        }
    } else {
        node->data = value;
        movetohead(obj, node);
    }
}

void printLRUCache(LRUCache *lru) {
    Qnode *current = lru->queue->head->next;
    printf("LRUCache: ");
    while (current != lru->queue->tail) {
        printf("(%d, %s) ", current->key.operation_number, current->key.name_of_file_or_folder);
        current = current->next;
    }
    printf("\n");
}

void freeLRUCache(LRUCache *obj) {
    while (obj->queue->head) {
        Qnode *temp = obj->queue->head;
        obj->queue->head = obj->queue->head->next;
        free(temp);
    }
    free(obj->queue);
    free(obj->hash);
    free(obj);
}

int main() {
    int capacity;
    FileNameAndOperation key;
    FileData value;

    printf("Enter the capacity of the LRUCache: ");
    scanf("%d", &capacity);

    LRUCache *lruCache = lRUCacheCreate(capacity);

    while (1) {
        printf("1. Put\n2. Get\n3. Print\n4. Exit\n");
        int choice;
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter operation_number, and name_of_file_or_folder to put in LRUCache: ");
                scanf("%d %s", &key.operation_number, key.name_of_file_or_folder);
                printf("Enter file size: ");
                scanf("%d", &value.size);
                lRUCachePut(lruCache, key, value);
                break;
            case 2:
                printf("Enter operation_number and name_of_file_or_folder to get from LRUCache: ");
                scanf("%d %s", &key.operation_number, key.name_of_file_or_folder);
                FileData data = lRUCacheGet(lruCache, key);
                printf("File Size: %d\n", data.size);
                break;
            case 3:
                printLRUCache(lruCache);
                break;
            case 4:
                freeLRUCache(lruCache);
                return 0;
            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }
    }
}

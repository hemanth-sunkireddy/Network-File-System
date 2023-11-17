#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASH_SIZE 10001
#define MAX_LENGTH 100

typedef struct FileNameAndOperation {
    int operation_number;
    char name_of_file_or_folder[MAX_LENGTH];
} FileNameAndOperation;

typedef struct NS_reply {
    int port;
    char IP[1024];
    int SS_no;
} NS_reply;

typedef struct FileData {
    NS_reply ns_reply_data;
} FileData;

typedef struct QNode {
    struct QNode *prev;
    struct QNode *next;
    FileNameAndOperation key;
    FileData data;
} QNode;

typedef struct Queue {
    QNode *head;
    QNode *tail;
    int count;
} Queue;

typedef struct {
    int capacity;
    Queue *queue;
    QNode **hash;
} LRUCache;

QNode *createNewNode(FileNameAndOperation key, FileData data) {
    QNode *temp = (QNode *)malloc(sizeof(QNode));
    temp->prev = NULL;
    temp->next = NULL;
    temp->key = key;
    temp->data = data;
    return temp;
}

Queue *createQueue(int numberOfFrames) {
    Queue *queue = (Queue *)malloc(sizeof(Queue));
    queue->head = createNewNode((FileNameAndOperation){-1, ""}, (FileData){{0}});
    queue->tail = createNewNode((FileNameAndOperation){-1, ""}, (FileData){{0}});
    queue->head->next = queue->tail;
    queue->tail->prev = queue->head;
    queue->count = 0;
    return queue;
}

QNode **createHash(int size) {
    QNode **hash = (QNode **)malloc(size * sizeof(QNode *));
    memset(hash, 0, size * sizeof(QNode *));
    return hash;
}

void removeNode(LRUCache *cache, QNode *node) {
    QNode *prev = node->prev;
    QNode *next = node->next;
    prev->next = next;
    next->prev = prev;
}

void addNode(LRUCache *cache, QNode *node) {
    node->prev = cache->queue->head;
    node->next = cache->queue->head->next;
    cache->queue->head->next->prev = node;
    cache->queue->head->next = node;
}

void moveToHead(LRUCache *cache, QNode *node) {
    removeNode(cache, node);
    addNode(cache, node);
}

QNode *popTail(LRUCache *cache) {
    QNode *node = cache->queue->tail->prev;
    removeNode(cache, node);
    return node;
}

LRUCache *lRUCacheCreate(int capacity) {
    LRUCache *lru = (LRUCache *)malloc(sizeof(LRUCache));
    lru->hash = createHash(HASH_SIZE);
    lru->queue = createQueue(capacity);
    lru->capacity = capacity;
    return lru;
}

FileData lRUCacheGet(LRUCache *obj, FileNameAndOperation key) {
    QNode *current = obj->queue->head->next;
    while (current != obj->queue->tail) {
        if (current->key.operation_number == key.operation_number &&
            strcmp(current->key.name_of_file_or_folder, key.name_of_file_or_folder) == 0) {
            // Key found, move to the head and return the data
            moveToHead(obj, current);
            return current->data;
        }
        current = current->next;
    }
    
    // Key not found, return a default value or handle accordingly
    return (FileData){{0}};
}


void lRUCachePut(LRUCache *obj, FileNameAndOperation key, FileData value) {
    QNode *node = obj->hash[key.operation_number];
    if (!node) {
        node = createNewNode(key, value);
        obj->hash[key.operation_number] = node;
        addNode(obj, node);
        obj->queue->count++;

        if (obj->queue->count > obj->capacity) {
            QNode *tail = popTail(obj);
            obj->hash[tail->key.operation_number] = NULL;
            obj->queue->count--;
            free(tail);
        }
    } else {
        node->data = value;
        moveToHead(obj, node);
    }
}

void printLRUCache(LRUCache *lru) {
    QNode *current = lru->queue->head->next;
    printf("LRUCache: ");
    while (current != lru->queue->tail) {
        printf("(%d, %s, %d, %s, %d) ", current->key.operation_number, current->key.name_of_file_or_folder,
               current->data.ns_reply_data.port, current->data.ns_reply_data.IP,
               current->data.ns_reply_data.SS_no);

        current = current->next;
    }
    printf("\n");
}

void freeLRUCache(LRUCache *obj) {
    while (obj->queue->head) {
        QNode *temp = obj->queue->head;
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
                printf("Enter operation number, name of file or folder, port, IP, and SS number to put in LRUCache: ");
                scanf("%d %s %d %s %d", &key.operation_number, key.name_of_file_or_folder, &value.ns_reply_data.port,
                      value.ns_reply_data.IP, &value.ns_reply_data.SS_no);
                lRUCachePut(lruCache, key, value);
                break;
            case 2:
                printf("Enter operation number and name of file or folder to get from LRUCache: ");
                scanf("%d %s", &key.operation_number, key.name_of_file_or_folder);
                FileData result = lRUCacheGet(lruCache, key);
                printf("Value for key %d: (%d, %s, %d)\n", key.operation_number, result.ns_reply_data.port,
                       result.ns_reply_data.IP, result.ns_reply_data.SS_no);
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

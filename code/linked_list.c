#include "thread_worker_types.h"
#include "thread-worker.h"
#include "linked_list.h"

// initializes linked list object
struct LinkedList* createList() {
    struct LinkedList* newList = (struct LinkedList*)malloc(sizeof(struct LinkedList));
    if (newList != NULL) {
        newList->head = NULL;
        newList->count = 0;
    }
    return newList;
}

// prints out the number of items in the queue
void printCount(struct LinkedList* list) {
    printf("    NUMBER OF THREADS LEFT: %d\n", list->count);
}

int returnCount(struct LinkedList* list) {
    return list->count;
}

// prints out queue
void printList(struct LinkedList* list) {
    if (list->count == 0) {
        printf("The queue is empty.\n");
    } else {
        struct Node* ptr = list->head;
        while (ptr != NULL) {
            if (ptr->data->id == 1000) {
                printf("MAIN(%d) -> ", ptr->data->status);
            } else {
                printf("%d(%d) -> ", ptr->data->id, ptr->data->status);
            }
            ptr = ptr->next;
        }
        printf("END\n");
    }
}

// creates a new node and returns it; parameter is a TCB struct
void addToQueue(struct LinkedList* list, struct TCB* tcb) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    if (newNode == NULL) {
        perror("Memory allocation failed.\n");
        exit(1);
    }
    newNode->data = tcb;
    newNode->next = NULL;
    
    if (list->count == 0) {
        list->head = newNode;
    } else {
        struct Node* ptr = list->head;
        while (ptr->next != NULL) {
            ptr = ptr->next;
        }
        ptr->next = newNode;
    }
    list->count++;
}

// pops a node from the list
int pop(struct LinkedList* list, struct TCB* tcb) {
    if (list->count == 1) {
        list->head = NULL;
    } else {
        if (list->head->data->id == tcb->id) { // if list head is the one that needs to be popped
            struct Node* newHead = list->head->next;
            list->head->next = NULL;
            list->head = newHead;
        } else { // if list head does not need to be popped
            struct Node* before = list->head;
            struct Node* after;
            while (before->next->data->id != tcb->id) {
                before = before->next;
            }
            if (before->next->next == NULL) {
                before->next = NULL;
            } else {
                after = before->next->next;
                before->next = after;
            }
        }
    }
    list->count--;
    return 0;
}

// moves first node to the end
int popAndPlop(struct LinkedList* list) {
    if (list->head == NULL || list->head->next == NULL) {
        return 1;
    }
    struct Node* lastNode = list->head;
    struct Node* firstNode = list->head;
    while (lastNode->next != NULL) {
        lastNode = lastNode->next;
    }
    list->head = list->head->next;
    firstNode->next = NULL;
    lastNode->next = firstNode;
    return 0;
}

// returns the value of a new thread id
int newThreadId(struct LinkedList* list) {
    return list->count + 1;
}

// returns the head of the queue
struct TCB* returnHeadTCB(struct LinkedList* list) {
    return list->head->data;
}

// returns the head of the queue
struct Node* returnHeadNode(struct LinkedList* list) {
    return list->head;
}

// returns the last node of the queue
struct Node* returnLast(struct LinkedList* list) {
    struct Node* ptr = list->head;
    while (ptr->next != NULL) {
        ptr = ptr->next;
    }
    return ptr;
}

struct TCB* searchTCB(struct LinkedList* list, worker_t thread) {
    struct Node* ptr = list->head;
    while (ptr->data->id != thread) {
        ptr = ptr->next;
    }
    return ptr->data;
}
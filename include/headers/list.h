#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef enum { SINGLY, DOUBLY, SINGLY_CIRCULAR, DOUBLY_CIRCULAR } ListType;

typedef struct Node {
    struct Node* prev;
    void* data;
    struct Node* next;
} node;

typedef struct LinkedList {
    struct Node* head;
    ListType type;
    struct Node* tail;
    int size;
} list;



int addNode(list** l, void* data);
void removeNode(list* l, void* data);
void* getNodeData(list* l, int index);
void clearList(list* l);
void printList(list* l, void (*printFunc)(void*));
int createList(list** l, ListType type);
void destroyList(list* l);

void linkNodeSingly(list** l, node* newNode);
void linkNodeDoubly(list** l, node* newNode);
void linkNodeSinglyCircular(list** l, node* newNode);
void linkNodeDoublyCircular(list** l, node* newNode);

#endif
// include/list.h

#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> 

typedef struct Node {
    struct Node *prev;
    int data;
    struct Node *next;
} Node;

// --- FUNCTION PROTOTYPES ---

// ### FRONTEND (menu_io.c) ###

void printMainMenu();
void handleMenu(Node **head);
void displayList(Node *head);
void handleSearchOperation(Node *head);

// ### BACKEND (list_logic.c) ###

Node* createList();
void sortList(Node **head);
bool searchNumber(Node *head, int number);
void freeList(Node **head);

// ### (optional enhancements) ###
// void reverseList(Node **head);
// bool deleteNode(Node **head, int number);
// int saveListToFile(Node *head, const char *filename);
// Node* loadListFromFile(const char *filename);


#endif
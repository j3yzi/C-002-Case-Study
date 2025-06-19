#include "../../include/list.h"

void linkNodeSingly(list** l, node* newNode) {
    if ((*l)->head == NULL) {
        (*l)->head = newNode;
        (*l)->tail = newNode;
    } else {
        (*l)->tail->next = newNode;
        (*l)->tail = newNode;
    }
}

void linkNodeDoubly(list** l, node* newNode) {
    if ((*l)->head == NULL) {
        (*l)->head = newNode;
        (*l)->tail = newNode;
    } else {
        newNode->prev = (*l)->tail;
        (*l)->tail->next = newNode;
        (*l)->tail = newNode;
    }
}

void linkNodeSinglyCircular(list** l, node* newNode) {
    if ((*l)->head == NULL) {
        (*l)->head = newNode;
        (*l)->tail = newNode;
        newNode->next = newNode; // Point to itself
    } else {
        newNode->next = (*l)->head; // Point to head
        (*l)->tail->next = newNode; // Previous tail points to new node
        (*l)->tail = newNode; // Update tail
    }
}

void linkNodeDoublyCircular(list** l, node* newNode) {
    if ((*l)->head == NULL) {
        (*l)->head = newNode;
        (*l)->tail = newNode;
        newNode->next = newNode; // Point to itself
        newNode->prev = newNode; // Point to itself
    } else {
        // Set the new node's next to the head (circular link)
        newNode->next = (*l)->head;
        // Set the new node's prev to the current tail
        newNode->prev = (*l)->tail;
        // Update the current tail's next to point to the new node
        (*l)->tail->next = newNode;
        // Update the head's prev to point to the new node
        (*l)->head->prev = newNode;
        // Move the tail pointer to the new node
        (*l)->tail = newNode;
    }
}
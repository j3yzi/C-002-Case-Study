#include "../headers/list.h"

/**
 * @brief Links a new node into a singly linked list.
 * @param l A double pointer to the list structure.
 * @param newNode The new node to be linked into the list.
 */
void linkNodeSingly(list** l, node* newNode) {
    if ((*l)->head == NULL) {
        (*l)->head = newNode;
        (*l)->tail = newNode;
    } else {
        (*l)->tail->next = newNode;
        (*l)->tail = newNode;
    }
}

/**
 * @brief Links a new node into a doubly linked list.
 * @param l A double pointer to the list structure.
 * @param newNode The new node to be linked into the list.
 */
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

/**
 * @brief Links a new node into a singly circular linked list.
 * @param l A double pointer to the list structure.
 * @param newNode The new node to be linked into the list.
 */
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

/**
 * @brief Links a new node into a doubly circular linked list.
 * @param l A double pointer to the list structure.
 * @param newNode The new node to be linked into the list.
 */
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
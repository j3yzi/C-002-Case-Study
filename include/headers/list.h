/**
 * @file list.h
 * @brief Generic Linked List Data Structure Implementation
 * 
 * This header file provides a flexible, generic linked list implementation
 * that supports multiple list types (singly linked, doubly linked, circular)
 * and can store any type of data through void pointers. The implementation
 * is used throughout the PUP Information Management System for managing
 * collections of employees, students, and other data.
 * 
 * @author C002 - Group 1
 * @version 1.0
 * @date 2024
 */

#ifndef LIST_H
#define LIST_H

#include <stdio.h>      // For standard I/O operations
#include <stdlib.h>     // For memory allocation functions
#include <stdbool.h>    // For boolean data type support

/**
 * @enum ListType
 * @brief Defines the different types of linked lists supported
 * 
 * This enumeration specifies the various linking strategies available
 * for the linked list implementation. Each type has different performance
 * characteristics and use cases.
 */
typedef enum { 
    SINGLY,           // Singly linked list (forward links only)
    DOUBLY,           // Doubly linked list (forward and backward links)
    SINGLY_CIRCULAR,  // Singly linked circular list (last node points to first)
    DOUBLY_CIRCULAR   // Doubly linked circular list (bidirectional circular)
} ListType;

/**
 * @struct Node
 * @brief Individual node structure for the linked list
 * 
 * Represents a single node in the linked list. Contains pointers to
 * adjacent nodes and a void pointer to store data of any type.
 * The prev pointer is only used in doubly linked lists.
 */
typedef struct Node {
    struct Node* prev;  // Pointer to previous node (NULL for singly linked)
    void* data;         // Pointer to the actual data stored in this node
    struct Node* next;  // Pointer to next node (NULL for end of list)
} node;

/**
 * @struct LinkedList
 * @brief Main linked list structure
 * 
 * Contains all the metadata and pointers necessary to manage a linked list.
 * Tracks the list type, head/tail pointers, and current size for efficient
 * operations and memory management.
 */
typedef struct LinkedList {
    struct Node* head;  // Pointer to the first node in the list
    ListType type;      // Type of linked list (singly, doubly, circular)
    struct Node* tail;  // Pointer to the last node in the list
    int size;           // Current number of nodes in the list
} list;

/**
 * @name Core List Operations
 * @brief Fundamental functions for list manipulation
 * @{
 */

/**
 * @brief Adds a new node with data to the end of the list
 * 
 * Creates a new node containing the provided data and adds it to the
 * end of the specified list. The linking behavior depends on the list type.
 * 
 * @param l Double pointer to the list (allows modification of list pointer)
 * @param data Pointer to the data to store in the new node
 * @return 0 on success, non-zero on error
 */
int addNode(list** l, void* data);

/**
 * @brief Removes a node containing specific data from the list
 * 
 * Searches for a node containing the specified data and removes it from
 * the list. Uses the provided function to free the data memory safely.
 * 
 * @param l Pointer to the list to modify
 * @param data Pointer to the data to find and remove
 * @param freeData Function pointer to properly free the data memory
 */
void removeNode(list* l, const void* data, void (*freeData)(void* data));

/**
 * @brief Retrieves data from a node at a specific index
 * 
 * Returns a pointer to the data stored in the node at the specified
 * index position. Index is 0-based. Returns NULL if index is out of bounds.
 * 
 * @param l Pointer to the list to search
 * @param index 0-based index of the node to retrieve
 * @return Pointer to the data, or NULL if not found
 */
void* getNodeData(const list* l, int index);

/**
 * @brief Removes all nodes from the list without destroying the list structure
 * 
 * Clears all nodes from the list and frees their memory using the provided
 * function. The list structure itself remains valid and can be reused.
 * 
 * @param l Pointer to the list to clear
 * @param freeData Function pointer to properly free each node's data
 */
void clearList(list* l, void (*freeData)(void* data));

/**
 * @brief Prints all data in the list using a custom print function
 * 
 * Iterates through all nodes in the list and calls the provided print
 * function for each node's data. Useful for debugging and display purposes.
 * 
 * @param l Pointer to the list to print
 * @param printFunc Function pointer to print individual data items
 */
void printList(const list* l, void (*printFunc)(const void* data));

/** @} */ // End of Core List Operations

/**
 * @name List Management Functions
 * @brief Functions for creating and destroying lists
 * @{
 */

/**
 * @brief Creates a new empty linked list of the specified type
 * 
 * Allocates memory for a new list structure and initializes it according
 * to the specified list type. The list is ready for use after creation.
 * 
 * @param l Double pointer to store the new list (allows modification of pointer)
 * @param type Type of linked list to create (singly, doubly, circular)
 * @return 0 on success, non-zero on error
 */
int createList(list** l, ListType type);

/**
 * @brief Destroys a list and frees all associated memory
 * 
 * Removes all nodes from the list, frees their data using the provided
 * function, and then frees the list structure itself. Sets the list
 * pointer to NULL to prevent dangling pointer issues.
 * 
 * @param l Double pointer to the list to destroy
 * @param freeData Function pointer to properly free each node's data
 */
void destroyList(list** l, void (*freeData)(void* data));

/** @} */ // End of List Management Functions

/**
 * @name Internal Linking Functions
 * @brief Low-level functions for different linking strategies
 * 
 * These functions handle the specific linking logic for different list types.
 * They are typically called internally by the higher-level list functions
 * and should not be called directly by user code.
 * @{
 */

/**
 * @brief Links a new node into a singly linked list
 * 
 * Handles the pointer manipulation required to add a node to a singly
 * linked list. Updates head and tail pointers as necessary.
 * 
 * @param l Double pointer to the list
 * @param newNode Pointer to the node to link into the list
 */
void linkNodeSingly(list** l, node* newNode);

/**
 * @brief Links a new node into a doubly linked list
 * 
 * Handles the pointer manipulation required to add a node to a doubly
 * linked list. Updates both forward and backward links.
 * 
 * @param l Double pointer to the list
 * @param newNode Pointer to the node to link into the list
 */
void linkNodeDoubly(list** l, node* newNode);

/**
 * @brief Links a new node into a singly linked circular list
 * 
 * Handles the pointer manipulation required to add a node to a singly
 * linked circular list. Maintains the circular property.
 * 
 * @param l Double pointer to the list
 * @param newNode Pointer to the node to link into the list
 */
void linkNodeSinglyCircular(list** l, node* newNode);

/**
 * @brief Links a new node into a doubly linked circular list
 * 
 * Handles the pointer manipulation required to add a node to a doubly
 * linked circular list. Maintains both circular and bidirectional properties.
 * 
 * @param l Double pointer to the list
 * @param newNode Pointer to the node to link into the list
 */
void linkNodeDoublyCircular(list** l, node* newNode);

/** @} */ // End of Internal Linking Functions

#endif // LIST_H
#include <stdio.h>
#include <stdlib.h>
#include "../headers/list.h"

/**
 * @brief Adds a new node with the given data to the end of the list.
 * @param l A double pointer to the list structure.
 * @param data A void pointer to the data to be stored in the new node.
 * @return Returns 0 on success, -1 on memory allocation failure.
 */
int addNode(list** l, void* data) {
    node* newNode = (node*)malloc(sizeof(node));
    
    if (!newNode) {
        fprintf(stderr, "Memory allocation failed\n");
        return -1;
    }

    newNode->data = data;
    newNode->next = NULL;
    newNode->prev = NULL;    
    switch ((*l)->type) {
        case SINGLY: {
            linkNodeSingly(l, newNode);
            break;
        }
        case DOUBLY: {
            linkNodeDoubly(l, newNode);
            break;
        }
        case SINGLY_CIRCULAR: {
            linkNodeSinglyCircular(l, newNode);
            break;
        }
        case DOUBLY_CIRCULAR: {
            linkNodeDoublyCircular(l, newNode);
            break;
        }
    }

    (*l)->size++;
    return 0;
}

/**
 * @brief Removes the first node containing the specified data from the list.
 * @param l A pointer to the list structure.
 * @param data A pointer to the data of the node to be removed. The comparison is by pointer address.
 * @param freeData A function pointer to a function that can free the data stored in the node. Can be NULL if data should not be freed.
 */
void removeNode(list* l, const void* data, void (*freeData)(void* data)) {
    if (l == NULL || l->head == NULL || data == NULL) {
        return;
    }
    
    node* current = l->head;
    node* prev = NULL;
    
    // Find the node with matching data
    while (current != NULL && current->data != data) {
        prev = current;
        current = current->next;
        
        // Handle circular lists to avoid infinite loops
        if (l->type == SINGLY_CIRCULAR || l->type == DOUBLY_CIRCULAR) {
            if (current == l->head) {
                // We've come full circle without finding the data
                return;
            }
        }
    }
    
    if (current == NULL) {
        // Node not found
        return;
    }
    
    // Handle the node removal based on list type
    switch (l->type) {
        case SINGLY:
            if (prev == NULL) {
                // Head node
                l->head = current->next;
                if (l->head == NULL) {
                    l->tail = NULL;
                }
            } else {
                prev->next = current->next;
                if (current == l->tail) {
                    l->tail = prev;
                }
            }
            break;
            
        case DOUBLY:
            if (prev == NULL) {
                // Head node
                l->head = current->next;
                if (l->head) {
                    l->head->prev = NULL;
                } else {
                    l->tail = NULL;
                }
            } else {
                prev->next = current->next;
                if (current->next) {
                    current->next->prev = prev;
                }
                if (current == l->tail) {
                    l->tail = prev;
                }
            }
            break;
            
        case SINGLY_CIRCULAR:
            if (prev == NULL) {
                // Head node in circular list
                if (current->next == current) {
                    // Single node in the list
                    l->head = NULL;
                    l->tail = NULL;
                } else {
                    l->head = current->next;
                    l->tail->next = l->head;
                }
            } else {
                prev->next = current->next;
                if (current == l->tail) {
                    l->tail = prev;
                }
            }
            break;
            
        case DOUBLY_CIRCULAR:
            if (current->next == current) {
                // Single node in the list
                l->head = NULL;
                l->tail = NULL;
            } else {
                current->prev->next = current->next;
                current->next->prev = current->prev;
                
                if (current == l->head) {
                    l->head = current->next;
                }
                if (current == l->tail) {
                    l->tail = current->prev;
                }
            }
            break;
    }
    
    if (freeData != NULL) {
        freeData(current->data);
    }
    free(current);
    l->size--;
}

/**
 * @brief Retrieves the data from a node at a specific index in the list.
 * @param l A const pointer to the list structure.
 * @param index The zero-based index of the node.
 * @return A void pointer to the data of the node at the specified index, or NULL if the index is out of bounds.
 */
void* getNodeData(const list* l, int index) {
    if (l == NULL || l->head == NULL || index < 0 || index >= l->size) {
        return NULL;
    }

    node* current = l->head;
    for (int i = 0; i < index; i++) {
        current = current->next;
        if (current == NULL) {
            return NULL;
        }
    }
    
    return current->data;
}

/**
 * @brief Removes all nodes from the list, optionally freeing the data in each node.
 * @param l A pointer to the list structure to be cleared.
 * @param freeData A function pointer to a function that can free the data stored in each node. Can be NULL.
 */
void clearList(list* l, void (*freeData)(void* data)) {
    if (l == NULL || l->head == NULL) {
        return;
    }
    
    node* current = l->head;
    node* next;
    
    // For circular lists, we need to break the circle first
    if (l->type == SINGLY_CIRCULAR || l->type == DOUBLY_CIRCULAR) {
        l->tail->next = NULL;
    }
    
    while (current != NULL) {
        next = current->next;
        if (freeData != NULL) {
            freeData(current->data);
        }
        free(current);
        current = next;
    }
    
    l->head = NULL;
    l->tail = NULL;
    l->size = 0;
}

/**
 * @brief Prints the contents of the list to the console.
 * @param l A const pointer to the list structure to be printed.
 * @param printFunc A function pointer to a function that knows how to print the data stored in a node.
 */
void printList(const list* l, void (*printFunc)(const void* data)) {
    if (l == NULL || l->head == NULL || printFunc == NULL) {
        printf("Empty list or invalid function pointer\n");
        return;
    }
    
    node* current = l->head;
    int count = 0;
    
    printf("List contents (%d items):\n", l->size);
    
    do {
        printf("Node %d: ", count++);
        printFunc(current->data);
        printf("\n");
        
        current = current->next;
        
        // For non-circular lists, stop at the end
        if ((l->type == SINGLY || l->type == DOUBLY) && current == NULL) {
            break;
        }
        
        // For circular lists, stop when we reach the head again
        if ((l->type == SINGLY_CIRCULAR || l->type == DOUBLY_CIRCULAR) && current == l->head) {
            break;
        }
    } while (current != NULL);
}

/**
 * @brief Creates and initializes a new list of a specified type.
 * @param l A double pointer to the list structure to be created.
 * @param type The type of the list to create (e.g., SINGLY, DOUBLY_CIRCULAR).
 * @return Returns 0 on success, -1 on memory allocation failure.
 */
int createList(list** l, ListType type) {
    *l = (list*)malloc(sizeof(list));
    if (!(*l)) {
        fprintf(stderr, "Memory allocation failed\n");
        return -1;
    }
    (*l)->head = NULL;
    (*l)->tail = NULL;
    (*l)->type = type;
    (*l)->size = 0;
    return 0;
}

/**
 * @brief Destroys a list, freeing all its nodes and optionally the data within them.
 * @param l A double pointer to the list structure to be destroyed. The pointer will be set to NULL after destruction.
 * @param freeData A function pointer to a function that can free the data stored in each node. Can be NULL.
 */
void destroyList(list** l, void (*freeData)(void* data)) {
    if (l == NULL || *l == NULL) {
        return;
    }
    
    // Clear all nodes and their data
    clearList(*l, freeData);
    
    // Free the list structure itself
    free(*l);
    *l = NULL; // Set the original pointer to NULL
}
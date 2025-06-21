#include "../headers/list.h"

void addNode(list** l, void* data) {
    node* newNode = (node*)malloc(sizeof(node));
    
    if (!newNode) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
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
}

void removeNode(list* l, void* data) {
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
    
    free(current);
    l->size--;
}

void* getNodeData(list* l, int index) {
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

void clearList(list* l) {
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
        free(current);
        current = next;
    }
    
    l->head = NULL;
    l->tail = NULL;
    l->size = 0;
}

void printList(list* l, void (*printFunc)(void*)) {
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

list* createList(ListType type) {
    list* newList = (list*)malloc(sizeof(list));
    if (!newList) {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }
    newList->head = NULL;
    newList->tail = NULL;
    newList->type = type;
    newList->size = 0;
    return newList;
}

void destroyList(list* l) {
    if (l == NULL) {
        return;
    }
    
    // Clear all nodes
    clearList(l);
    
    // Free the list structure itself
    free(l);
}
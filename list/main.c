#include <stdio.h>

#include "list.h"

typedef struct {
    int id;
    char name[50];
} employee;

void printEmployee(void* data) {
    if (data == NULL) {
        return;
    }

    employee* emp = (employee*)data;

    printf("\t%s\t%d",
           emp->name,
           emp->id);
}

int main (void) {
    employee *emp1 = malloc(sizeof(employee));
    emp1->id = 1;
    snprintf(emp1->name, sizeof(emp1->name), "Alice Guo");

    employee *emp2 = malloc(sizeof(employee));
    emp2->id = 2;
    snprintf(emp2->name, sizeof(emp2->name), "Wahafen vella?");

    employee *emp3 = malloc(sizeof(employee));
    emp3->id = 3;
    snprintf(emp3->name, sizeof(emp3->name), "Mga marc talaga");

    list *mylist = createList(DOUBLY_CIRCULAR);

    addNode(&mylist, emp1);
    addNode(&mylist, emp2);
    addNode(&mylist, emp3);

    printf("List size after adding one employee: %d\n", mylist->size);
    printList(mylist, printEmployee);

    printf("\n\n%s -> %s -> %s\n",
           ((employee*)mylist->head->data)->name,
           ((employee*)mylist->head->next->data)->name,
           ((employee*)mylist->tail->data)->name);

    printf("\n\nPREV: %s\tHEAD: %s\t\tNEXT: %s\n",((employee*)mylist->head->prev->data)->name, ((employee*)mylist->head->data)->name, ((employee*)mylist->head->next->data)->name);
    printf("PREV: %s\t\tMID: %s\tNEXT: %s\n",((employee*)mylist->head->next->prev->data)->name, ((employee*)mylist->head->next->data)->name, ((employee*)mylist->head->next->next->data)->name);
    printf("PREV: %s\tTAIL: %s\tNEXT: %s\n",((employee*)mylist->tail->prev->data)->name, ((employee*)mylist->tail->data)->name, ((employee*)mylist->tail->next->data)->name);

    return 0;
}
#ifndef LIST_H
#define LIST_H

// Node structure for the linked list
typedef struct node {
    int data;
    struct node *next;
} node_t;

// List structure
typedef struct {
    node_t *head;
    int length;
} list_t;

// Function prototypes
list_t* list_alloc();
void list_free(list_t *list);

int list_length(list_t *list);
char* listToString(list_t *list);

int list_add_to_back(list_t *list, int val);
int list_add_to_front(list_t *list, int val);
int list_add_at_index(list_t *list, int val, int index);

int list_remove_from_back(list_t *list, int *val);
int list_remove_from_front(list_t *list, int *val);
int list_remove_at_index(list_t *list, int *val, int index);

int list_get_elem_at(list_t *list, int *val, int index);

#endif // LIST_H
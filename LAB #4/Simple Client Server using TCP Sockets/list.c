#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

// Allocate a new list
list_t* list_alloc() {
    list_t *new_list = malloc(sizeof(list_t));
    if (new_list == NULL) {
        return NULL;
    }
    new_list->head = NULL;
    new_list->length = 0;
    return new_list;
}

// Free the entire list
void list_free(list_t *list) {
    if (list == NULL) {
        return;
    }
    
    node_t *current = list->head;
    while (current != NULL) {
        node_t *temp = current;
        current = current->next;
        free(temp);
    }
    
    free(list);
}

// Get the length of the list
int list_length(list_t *list) {
    if (list == NULL) {
        return 0;
    }
    return list->length;
}

// Convert list to string representation
char* listToString(list_t *list) {
    if (list == NULL || list->length == 0) {
        return strdup("List is empty");
    }

    // Allocate initial buffer
    size_t buffer_size = 1024;
    char *buffer = malloc(buffer_size);
    if (!buffer) {
        return NULL;
    }

    // Start with an empty string
    buffer[0] = '\0';
    node_t *current = list->head;
    int offset = 0;

    // Iterate through list and append each value
    while (current != NULL) {
        // Check if we need to resize the buffer
        if (offset + 20 > buffer_size) {  // Leave room for potential number and separator
            buffer_size *= 2;
            char *new_buffer = realloc(buffer, buffer_size);
            if (!new_buffer) {
                free(buffer);
                return NULL;
            }
            buffer = new_buffer;
        }

        // Append current value
        offset += snprintf(buffer + offset, buffer_size - offset, 
                           "%d%s", current->data, 
                           current->next ? " -> " : "");
        
        current = current->next;
    }

    return buffer;
}

// Add to back of the list
int list_add_to_back(list_t *list, int val) {
    if (list == NULL) {
        return -1;
    }

    node_t *new_node = malloc(sizeof(node_t));
    if (new_node == NULL) {
        return -1;
    }
    
    new_node->data = val;
    new_node->next = NULL;

    if (list->head == NULL) {
        list->head = new_node;
    } else {
        node_t *current = list->head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_node;
    }

    list->length++;
    return 0;
}

// Add to front of the list
int list_add_to_front(list_t *list, int val) {
    if (list == NULL) {
        return -1;
    }

    node_t *new_node = malloc(sizeof(node_t));
    if (new_node == NULL) {
        return -1;
    }
    
    new_node->data = val;
    new_node->next = list->head;
    list->head = new_node;
    list->length++;
    return 0;
}

// Add at specific index
int list_add_at_index(list_t *list, int val, int index) {
    if (list == NULL || index < 0 || index > list->length) {
        return -1;
    }

    // If adding at the front
    if (index == 0) {
        return list_add_to_front(list, val);
    }

    node_t *new_node = malloc(sizeof(node_t));
    if (new_node == NULL) {
        return -1;
    }
    
    new_node->data = val;

    node_t *current = list->head;
    for (int i = 0; i < index - 1; i++) {
        current = current->next;
    }

    new_node->next = current->next;
    current->next = new_node;
    list->length++;
    return 0;
}

// Remove from back of the list
int list_remove_from_back(list_t *list, int *val) {
    if (list == NULL || list->head == NULL) {
        return -1;
    }

    // If only one element
    if (list->head->next == NULL) {
        *val = list->head->data;
        free(list->head);
        list->head = NULL;
        list->length--;
        return 0;
    }

    // Traverse to second-to-last node
    node_t *current = list->head;
    while (current->next->next != NULL) {
        current = current->next;
    }

    *val = current->next->data;
    free(current->next);
    current->next = NULL;
    list->length--;
    return 0;
}

// Remove from front of the list
int list_remove_from_front(list_t *list, int *val) {
    if (list == NULL || list->head == NULL) {
        return -1;
    }

    node_t *temp = list->head;
    *val = temp->data;
    list->head = list->head->next;
    free(temp);
    list->length--;
    return 0;
}

// Remove at specific index
int list_remove_at_index(list_t *list, int *val, int index) {
    if (list == NULL || list->head == NULL || 
        index < 0 || index >= list->length) {
        return -1;
    }

    // If removing from front
    if (index == 0) {
        return list_remove_from_front(list, val);
    }

    node_t *current = list->head;
    for (int i = 0; i < index - 1; i++) {
        current = current->next;
    }

    node_t *temp = current->next;
    *val = temp->data;
    current->next = temp->next;
    free(temp);
    list->length--;
    return 0;
}

// Get element at specific index
int list_get_elem_at(list_t *list, int *val, int index) {
    if (list == NULL || list->head == NULL || 
        index < 0 || index >= list->length) {
        return -1;
    }

    node_t *current = list->head;
    for (int i = 0; i < index; i++) {
        current = current->next;
    }

    *val = current->data;
    return 0;
}
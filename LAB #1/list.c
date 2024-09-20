// list/list.c
// 
// Implementation for linked list.
//
// <Author>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"

// Allocating memory for the list and initializing the head
list_t *list_alloc() { 
  list_t* mylist =  (list_t *) malloc(sizeof(list_t)); 
  mylist->head = NULL;
  return mylist;
}

// Free the momory allocated for the list
void list_free(list_t *l) {
  node_t *curr = l->head;
  node_t *next;

  while(curr != NULL) {
    next = curr->next;
    free(curr);
    curr = next;
  }
  free(l);
  }

// Print the entire list
void list_print(list_t *l) {
  node_t *curr = l->head;
  while (curr !=NULL) {
    printf("%d -> ", curr->value);
    curr =curr->next;
  }
  printf("NULL\n");
}

// Convert the list to a string representation
char * listToString(list_t *l) {
  char* buf = (char *) malloc(sizeof(char) * 1024);
  char tbuf[20];

	node_t* curr = l->head;
  while (curr != NULL) {
    sprintf(tbuf, "%d->", curr->value);
    curr = curr->next;
    strcat(buf, tbuf);
  }
  strcat(buf, "NULL");
  return buf;
}

// Returns the length of the list
int list_length(list_t *l) { 
  int count = 0;
  node_t *curr = l->head;
  while (curr != NULL) {
    count++;
    curr = curr->next;
  }
  return count;
}

// Add an element to the back of the list
void list_add_to_back(list_t *l, elem value) {
  node_t *new_node = (node_t *) malloc(sizeof(node_t));
  new_node->value = value;
  new_node->next =NULL;

  if (l->head ==NULL) {
    l->head = new_node;
  } else {
    node_t *curr = l->head;
    while (curr->next != NULL) {
      curr = curr->next;
    }
    curr->next = new_node;
  }
}

// Add an element to the front of the list
void list_add_to_front(list_t *l, elem value) {
  node_t* cur_node = (node_t *) malloc(sizeof(node_t));
  cur_node->value = value;
     

  /* Insert to front */
  cur_node->next = l->head;
  l->head = cur_node;

}

// Add an element at a specific index
void list_add_at_index(list_t *l, elem value, int index) {
  if (index <0) return;

  if (index ==0) {
    list_add_to_front(l, value);
    return;
  }

  node_t *new_node = (node_t *) malloc(sizeof(node_t));
  new_node->value = value;

  node_t *curr = l->head;
  int i;
  for (i =0; i< index - 1 && curr != NULL; i++) {
    curr = curr->next;
  }

  if (curr == NULL) return;

  new_node->next = curr->next;
  curr->next = new_node;
}

// Remove and return the last element
elem list_remove_from_back(list_t *l) {
  if (l->head ==NULL) return -1;
  
  node_t *curr = l->head;
  if (curr->next =NULL) {
    elem value =curr->value;
    free(curr);
    l->head =NULL;
    return value;
  }

  while (curr->next->next !=NULL){
    curr =curr->next;
  }
  elem value = curr->next->value;
  free(curr->next);
  curr->next =NULL;
  return value;
}

// Remove and return the first element
elem list_remove_from_front(list_t *l) {
  if (l->head ==NULL) return -1;

  node_t *first_node = l->head;
  elem value = first_node->value;
  l->head = l->head->next;
  free(first_node);
  return value;
}

// Remove an element at a specific index
elem list_remove_at_index(list_t *l, int index) { 
  if (index<0 || l->head == NULL) return -1;

  if (index ==0) {
    return list_remove_from_front(l);
  }

  node_t *curr = l->head ;
  int i;
  for (i = 0; i<index - 1 && curr->next != NULL; i++) {
    curr = curr->next;
  }

  if (curr->next == NULL) return -1;

  node_t *node_to_remove = curr->next;
  elem value = node_to_remove->value;
  curr->next = node_to_remove->next;
  free(node_to_remove);
  return value;
}

// Check if a value is in the list
bool list_is_in(list_t *l, elem value) { 
  node_t *curr = l->head;
  while (curr!=NULL) {
    if (curr->value == value) return true;
    curr = curr->next;
  } 
  return false;
}

// Get the element at a specific index
elem list_get_elem_at(list_t *l, int index) { 
  if (index <0) return -1;

  node_t *curr = l->head;
  int i;
  for (i=0; i<index && curr!=NULL; i++) {
    curr =curr->next;
  }
  if (curr == NULL) return -1;
  
  return curr->value;
}

// Get the index of a specific value in the list
int list_get_index_of(list_t *l, elem value) { 
  int index=0;
  node_t *curr =l->head;
  while (curr != NULL) {
    if(curr->value ==value) return index;
    curr = curr->next;
    index++;
  }
  return -1;
}


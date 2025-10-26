#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "linked_list.h"

Node* create_node(int value)
{
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (!new_node) return NULL;
    new_node->data = value;
    new_node->next = NULL;
    return new_node;

}
void append(Node** head, int value)
{
    Node* new_node = create_node(value);
    if (!new_node) return;
    if (*head == NULL) {
        *head = new_node;
        return;
    }
    Node* curr = *head;
    while (curr->next != NULL) {
        curr = curr->next;
    }
    curr->next = new_node;

}
void print_list(Node* head)
{
    Node* curr = head;
    while (curr) {
        printf("%d ", curr->data);
        curr = curr->next;
    }
    printf("\n");

}

Node* find(Node* head, int value)
{
    Node* curr = head;
    while (curr) {
        if (curr->data == value) return curr;
        curr = curr->next;
    }
    return NULL;

}
bool delete_node(Node** head, int value)
{
    if (!head){
        return false;
    }
    Node* curr = head;
    Node* prev = NULL;

    while(curr){
       if (curr->data == value){
        prev.next = curr.next;
        free(curr);
        return true;
       }

       prev = curr;
       curr = curr.next;
    
    }

}

void free_list(Node* head)
{
     Node* curr = head;
    while (curr) {
        Node* tmp = curr;
        curr = curr->next;
        free(tmp);
    }

}

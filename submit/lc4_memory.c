/************************************************************************/
/* File Name : lc4_memory.c             */
/* Purpose   : This file implements the linked_list helper functions */
/*       to manage the LC4 memory         */
/*                            */
/* Author(s) : tjf and you            */
/************************************************************************/

#include <stdio.h>
#include "lc4_memory.h"
#include <stdlib.h>


/*
 * adds a new node to a linked list pointed to by head
 */
int add_to_list (row_of_memory** head,
   short unsigned int address,
   short unsigned int contents)
{
    /* check to see if there is already an entry for this address and update the contents.  no additional steps required in this case */  
    /* allocate memory for a single node */
 /* populate fields in newly allocated node w/ address&contents, NULL for label and assembly */
 /* do not malloc() storage for label and assembly here - do it in parse_file() and reverse_assemble() */
 /* if *head is NULL, node created is the new head of the list! */
 /* otherwise, insert node into the list in address ascending order */
 /* return 0 for success, -1 if malloc fails */

     row_of_memory* found_node = search_address(*head, address);
    if (found_node != NULL) {
        // if address exist, update the contents
        found_node->contents = contents;
        return 0;
    }
    // create new node if no existing address
    row_of_memory* new_node = (row_of_memory*)malloc(sizeof(row_of_memory));
    if (new_node == NULL) {
        return -1; 
    }
    new_node->address = address;
    new_node->contents = contents;
    new_node->label = NULL;
    new_node->assembly = NULL;
    new_node->next = NULL; 

    //keep ascending order
    if (*head == NULL || (*head)->address > address) {
        new_node->next = *head;
        *head = new_node;
    } else {
        // find the end of list and link it with new node
        row_of_memory* current = *head;
        while (current->next != NULL && current->next->address < address) {
            current = current->next;
        }
        new_node->next = current->next;
        current->next = new_node;
    }
 return 0 ;
}



/*
 * search linked list by address field, returns node if found
 */
row_of_memory* search_address (row_of_memory* head,
          short unsigned int address )
{
 /* traverse linked list, searching each node for "address"  */
 /* return pointer to node in the list if item is found */
 /* return NULL if list is empty or if "address" isn't found */
  while (head != NULL) {
        if (head->address == address) {
            // return address of this node
            return head;
        }
        // go to next node
        head = head->next;
    }

 return NULL ;
}

/*
 * search linked list by opcode field, returns node if found
 */
row_of_memory* search_opcode  (row_of_memory* head,
          short unsigned int opcode  ){
    /* opcode parameter is in the least significant 4 bits of the short int and ranges from 0-15 */
  /* see assignment instructions for a detailed description */
    /* traverse linked list until node is found with matching opcode in the most significant 4 bits
    AND "assembly" field of node is NULL */
 /* return pointer to node in the list if item is found */
 /* return NULL if list is empty or if no matching nodes */

    while (head != NULL) {
        // get left most 4 bit of content
        unsigned short content_opcode = (head->contents >> 12) & 0xF;
        if (content_opcode == opcode && head->assembly == NULL) { // check if opcode are same and assembly still not assign
            return head;
        }
        head = head->next;
    }
    return NULL; //return null is no match
}


void print_list (row_of_memory* head ){
 /* make sure head isn't NULL */
 /* print out a header */
  /* don't print assembly directives for non opcode 1 instructions if you are doing extra credit */
 /* traverse linked list, print contents of each node */
  if (head == NULL) {
        printf("The list is empty.\n");
        return;
    }
    printf("%-20s %-20s %-20s %-20s\n", "<Label>", "<Address>", "<Contents>", "<Assembly>");

    while (head != NULL) {
    printf("%-20s", (head->label != NULL) ? head->label : ""); // if no label, leave it as blank
    printf("%04X ", head->address);
    printf("%04X ", (head->contents != 0) ? head->contents : 0);
    printf("%-20s", (head->assembly != NULL) ? head->assembly : "");
    printf("\n");
    head = head->next;
    }
}


/*
 * delete entire linked list
 */
int delete_list    (row_of_memory** head )
{
 /* delete entire list node by node */
 /* set the list head pointer to NULL upon deletion */
if (!*head) {
        return 0;
    }

    row_of_memory* current = *head;
    row_of_memory* next;

    while (current) {
        next = current->next; //before delete the current, store the pointer of next node first
        if (current->label) {
        free(current->label);
        current->label = NULL;
        }
        if (current->assembly) {
            free(current->assembly);
            current->assembly = NULL;
        }
        free(current);
        current = NULL;
        current = next;
    }
    *head = NULL;
    return 0;
}
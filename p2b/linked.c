// A simple C program for traversal of a linked list
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
struct Node {
    char *alias;
    char *path;
    struct Node *next;
};


// This function prints contents of linked list starting from
// the given node
void printList(struct Node* n)
{
    while (n != NULL) {
        printf(" %s ", n->alias);
        n = n->next;
    }
}

struct Node* search(struct Node* head, char* x)
{
    struct Node* current = head;  // Initialize current
    while (current != NULL)
    {
        if (current->alias == x)
            return current;
        current = current->next;
    }
    return NULL;
}

//insert a link with given key
struct Node* insert(struct Node* head, char* key) {

   //start from the first link
   struct Node* current = head;
   struct Node* previous = NULL;
	
   //if list is empty
//    if(head == NULL) {
//       return NULL;
//    }

   //navigate through list
   while(current->alias == key) {

      //if it is last node
      if(current->next == NULL) {
         return NULL;
      } else {
         //store reference to current link
         previous = current;
         //move to next link
         current = current->next;
      }
   }

   //found a match, update the link
   if(current == head) {
      //change first to point to next link
      head = head->next;
   } else {
      //bypass the current link
      previous->next = current->next;
   }    
	
   return current;
}

//delete a link with given key
struct Node* delete(struct Node* head, char *key) {

   //start from the first link
   struct Node* current = head;
   struct Node* previous = NULL;
	
   //if list is empty
   if(head == NULL) {
      return NULL;
   }

   //navigate through list
   while(current->alias != key) {

      //if it is last node
      if(current->next == NULL) {
         return NULL;
      } else {
         //store reference to current link
         previous = current;
         //move to next link
         current = current->next;
      }
   }

   //found a match, update the link
   if(current == head) {
      //change first to point to next link
      head = head->next;
   } else {
      //bypass the current link
      previous->next = current->next;
   }    
	
   return current;
}

int main()
{
    char s[] = "ls";
    char p[] = "/bin/ls";
	struct Node node1 = {s, p, NULL};
	struct Node* head = &node1;
    struct Node* second = NULL;
    struct Node* third = NULL;
    struct Node* result = NULL;
 
    // allocate 3 nodes in the heap
    //head = (struct Node*)malloc(sizeof(struct Node));
    second = (struct Node*)malloc(sizeof(struct Node));
    third = (struct Node*)malloc(sizeof(struct Node));
 
    // head->alias = 1; // assign alias in first node
    head->next = second; // Link first node with second
 
    second->alias = s; // assign alias to second node
    second->next = third;
 
    third->alias = s; // assign alias to third node
    third->next = NULL;
 
    printList(head);
    result = search(head, s);
    printf("search result is %s\n", result->alias);
    // delete(head, 3);
    result = search(head, "100");
    if (result == NULL)
        printf("search result is NULL");
 
    return 0;
}
// A simple C program for traversal of a linked list
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
struct Node {
    char *alias;
    char *path;
    struct Node *next;
};

struct Linkedlist {
    struct Node *head;
};

// This function prints contents of linked list starting from
// the given node
void printList(struct Linkedlist* ll){
   struct Node *n = ll->head;
   while (n != NULL) {
      printf("%s %s\n", n->alias, n->path);
      n = n->next;
   }
}

void freeList(struct Linkedlist* ll){
   struct Node *n = ll->head;
   struct Node *previous = NULL;
   while (n != NULL) {
      previous = n;
      n = n->next;
      free(previous);
   }
   ll->head = NULL;
   printf("Free completed.");
}

struct Node* search(struct Linkedlist* ll, char* x){
   struct Node *head = ll->head;
   struct Node* current = head;  // Initialize current
   while (current != NULL) {
      if (strcmp(current->alias, x) == 0)
         return current;
      current = current->next;
   }
   return NULL;
}

//insert a link with given key
int insert(struct Linkedlist* ll, struct Node *new) {

   // printf("Insert started! Node is %s, %s\n", new->alias, new->path);
   //start from the first link
   struct Node* head = ll->head;
   struct Node* current = head;
   struct Node* previous = NULL;
   // int i = 1;
	
   //if list is empty
   if(head == NULL) {
      ll->head = new;
      // printList(ll);
      return 0;
   }
   
   //navigate through list
   while(current != NULL) {
      if (strcmp(current->alias, new->alias) == 0) {
         // printf("Replace! alias is %s, %s\n", current->alias, new->alias);
         current->path = new->path;
         return 0;
      }
      previous = current;
      current = current->next;
   }
   // printf("Not found match!\n");
   previous->next = new;
   return 0;
}

//delete a link with given key
struct Node* delete(struct Linkedlist *ll, char *key) {

   //start from the first link
   struct Node *head = ll->head;
   struct Node* current = head;
   struct Node* previous = NULL;
	
   //if list is empty
   if(head == NULL) {
      return NULL;
   }

   //navigate through list
   // printf("First alias is %s\n", current->alias);
   while(strcmp(current->alias, key) != 0) {
   // printf("While is executed");
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
      ll->head = current->next;
      free(current);
   } else {
      //bypass the current link
      previous->next = current->next;
      free(current);
   }    
	
   return current;
}

void addAlias(struct Linkedlist* ll, char* alias1, char* path1) {
   // printf("pointer of ll is:  %p\n", ll);
   // printf("original head of ll is:  %p\n", ll->head);
  struct Node* p;
  p = (struct Node*)malloc(sizeof(struct Node));
  p->alias = alias1;
  p->path = path1;
  p->next = NULL;
//   printf("This p is pointing to:  %p\n", p);
//   printf("After new p created, the pointer of ll is:  %p\n", ll);
//   printf("After new p created, the head of ll is:  %p\n", ll->head);
//   if (ll->head != NULL)
//       printf("After new p created, the content in ll's head is %s, %s", ll->head->alias,ll->head->path);
  // printList(ll);
  // printf("%s, %s\n", new->alias, new->path);
  insert(ll, p);
  // printList(&ll0);
}

// int main() {
//    char s[] = "ls";
//    char p[] = "/bin/ls";
//    // struct Node node1 = {s, p, NULL};
//    struct Node* head = NULL;
//    struct Node* second = NULL;
//    struct Node* third = NULL;
//    struct Node* result = NULL;

//    // struct Linkedlist* ll = (struct Linkedlist*)malloc(sizeof(struct Linkedlist));
   
//     // allocate 3 nodes in the heap
//     head = (struct Node*)malloc(sizeof(struct Node));
//     second = (struct Node*)malloc(sizeof(struct Node));
//     third = (struct Node*)malloc(sizeof(struct Node));
//    struct Linkedlist ll1 = {NULL};
//    ll1.head = head;
//     head->alias = s; // assign alias in first node
//     head->path = p;
//     head->next = NULL; // Link first node with second
 
//     second->alias = s; // assign alias to second node
//     second->next = third;
 
//     third->alias = s; // assign alias to third node
//     third->next = NULL;

//    // printf("Stage1!!!\n");
//    char s1[] = "cd";
//    char p1[] = "/bin/cd";
//    char s2[] = "100";
//    char p2[] = "100W";
//    char s3[] = "bs";
//    char p3[] = "bbs.xxx.com";
//    // struct Node* new = (struct Node*)malloc(sizeof(struct Node));
//    // new->alias = s1;
//    // new->path = p1;
//    // new->next = NULL;
//    //    printList(&ll1);
//    // // printf("Stage2!!!\n");   
//    addAlias(&ll1, s1, p1);
//    printList(&ll1);
//    addAlias(&ll1, s2, p2);
//    printList(&ll1);
//    addAlias(&ll1, s3, p3);
//    printList(&ll1);


//    // insert(&ll1, new);
//    //       //printf("Break1!!!\n"); 
//    // printList(&ll1);
//    // // printf("Stage3!!!\n"); 
   
//    // new->alias = s;
//    // insert(&ll1, new);
//    // printList(&ll1);
//    // delete(&ll1, s);
//    // printList(&ll1);
//    // // result = search(&ll1, s);
//    // // printf("search result is %s\n", result->alias);
//    // // delete(head, 3);


//    //  result = search(&ll1, s);
//    //  if (result == NULL)
//    //      printf("search result is NULL.\n");
//    // else {
//    //    printf("search result is %s, %s.\n", result->alias, result->path);
//    // }
//    freeList(&ll1);
//    printList(&ll1);
 
//     return 0;
// }
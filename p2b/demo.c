#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "exec_test.c"
#include "linked.c"

#define MAX 512
#define delim " \n"
#define TOK_BUFSIZE 64

struct Linkedlist ll0 = {NULL};

// void addAlias(char* alias, char* path) {
//   struct Node* p;
//   p = (struct Node*)malloc(sizeof(struct Node));
//   p->alias = alias;
//   p->path = path;
//   p->next = NULL;
//   // printf("%s, %s\n", new->alias, new->path);
//   insert(&ll0, p);
//   // printList(&ll0);
// }

void deleteAlias(struct Linkedlist* ll, char* alias) {
  delete(ll, alias);
}

void printAlias(struct Linkedlist* ll) {
  printList(ll);
}

void *mysh_read_line(char *buf){
  fgets(buf, MAX, stdin);
}

int mysh_parse_line(char *line, char **tokens, char *output){
  int i = 0;
  char *token;
  // printList(&ll0);
  token = strtok(line, delim);
    if (strcmp(token ,"exit") == 0)
      _exit(0);
    else if (strcmp(token ,"alias") == 0) {
      char *alias = strtok(NULL, delim);
      // printf("Remaining line is %s,\n", line);
      char *path = strtok(NULL, "\n");
      if (alias == NULL) {
        printAlias(&ll0);
        return 4;
      }
      else if (path == NULL) {
        struct Node* result = search(&ll0, alias);
        if (result != NULL) {
          printf("%s, %s\n", result->alias, result->path);
        }
        return 5;
      }
      if (strcmp(alias ,"alias")==0 || strcmp(alias ,"unalias")==0 || strcmp(alias ,"exit")==0) {
        fprintf(stderr, "alias: Too dangerous to alias that.\n");
        return 6;
      }
      // printf("%s, %s\n", alias, path);
      addAlias(&ll0, strdup(alias), strdup(path));
      return 2;     
    }
    else if (strcmp(token ,"unalias") == 0) {
      char *alias = strtok(NULL, delim);
      if (strtok(NULL, delim) != NULL) {
        printf("unalias: Incorrect number of arguments.\n");
        return 3;
      }
      // printf("%s\n", alias);
      deleteAlias(&ll0, alias);
      return 3;
    }
    // else if (strcmp(token ,"printalias") == 0) {
    //   printAlias(&ll0);
    //   return 4;
    // }  
    
    /* walk through other tokens */
    while( token != NULL ) {
      // printf( " %s\n", token );
      // status  = ();
      if (strcmp(token ,">") == 0) {
        // printf( " %s is found, return soon\n", token );
        // tokens[i] = token;
        i++;
        strcpy(output, strtok(NULL, delim));
        tokens[i] = NULL;
        // printf( " %s\n", token );
        return 1;
      }
      tokens[i] = token;
      i++;
      token = strtok(NULL, delim);
    }
    tokens[i] = NULL;
    return 0;
}

void mysh_loop(void)
{
  char line[MAX];
  // char **args;
  int status;

  char output[MAX];
  char **args = malloc(TOK_BUFSIZE * sizeof(char*));

  while (1) {
    int redir_flag = 0;
    printf("mysh> ");
    // printList(&ll0);
    mysh_read_line(line);
    redir_flag = mysh_parse_line(line, args, output);

    if (redir_flag > 1)
      continue;
    
    /* get the first token */
    // printf("%s, %s, %s\n", args[0], args[1], args[2]);
    // printf("output is %s\n", output);
    struct Node* full = search(&ll0, args[0]);
    if (full != NULL)
      args[0] = full->path;
    // printf("After translation command is %s\n", args[0]);
    mysh_execute(args, redir_flag, output);

    // args = mysh_split_line(line);
    // status = mysh_execute(args);

    // free(line);
    // printf("%s\n", line);
    
    // printf("%d\n", status);
    free(args);
  }
}

int main(int argc, char **argv)
{

  if (argc == 1) {
    // Run command loop.
    mysh_loop();
  } else if (argc == 2) {
    FILE *fp = fopen(argv[1], "r");
    if (fp == NULL) {
        printf("Error: Cannot open file %s.\n", argv[1]);
        exit(1);
    }
    char buf[MAX];
    char output[MAX];
    char **args = malloc(TOK_BUFSIZE * sizeof(char*));
    while (fgets(buf, MAX, fp)) {
      printf("%s", buf);
      int redir_flag = 0;
      redir_flag = mysh_parse_line(buf, args, output);

      if (redir_flag > 1)
        continue;
      
      /* get the first token */
      // printf("%s, %s, %s\n", args[0], args[1], args[2]);
      // printf("output is %s\n", output);
      struct Node* full = search(&ll0, args[0]);
      if (full != NULL)
        args[0] = full->path;
      // printf("After translation command is %s\n", args[0]);
      mysh_execute(args, redir_flag, output);
      
      // printf("%d\n", status);
      free(args);
      fclose(fp);
    }
  } else {
    printf("Usage: mysh [batch-file]\n");
  }

  // Perform any shutdown/cleanup.

  return 0;
}
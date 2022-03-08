#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "exec_test.c"
#include "linked.c"

#define MAX 512
#define XMAX 10000
#define delim " \t\r\n\v\f"
#define TOK_BUFSIZE 64

struct Linkedlist ll0 = {NULL};

void deleteAlias(struct Linkedlist* ll, char* alias) {
  delete(ll, alias);
}

void printAlias(struct Linkedlist* ll) {
  printList(ll);
}

int mysh_read_line(char *buf){
  char* line = fgets(buf, MAX, stdin);
  if (line == NULL)
    return 100;
  return 0;
}

int mysh_parse_line(char *line, char **tokens, char *output){
  int i = 0;
  char *token;
  
  token = strtok(line, delim);
  // printf( "current token is %s\n", token );
  // fflush(stdout);
  if (token == NULL)
    return 10;
  else if (strcmp(token ,"exit") == 0)
    return 100;
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
        printf("%s %s\n", result->alias, result->path);
        fflush(stdout);
      }
      return 5;
    }
    if (strcmp(alias ,"alias")==0 || strcmp(alias ,"unalias")==0 || strcmp(alias ,"exit")==0) {
      fprintf(stderr, "alias: Too dangerous to alias that.\n");
      fflush(stderr);
      return 6;
    }
    // printf("%s, %s\n", alias, path);
    struct Node* p = (struct Node*)malloc(sizeof(struct Node));
    p->alias = strdup(alias);
    p->path = strdup(path);
    p->next = NULL;
    insert(&ll0, p);
    return 2;     
  }
  else if (strcmp(token ,"unalias") == 0) {
    char *alias = strtok(NULL, delim);
    if (strtok(NULL, delim) != NULL) {
      printf("unalias: Incorrect number of arguments.\n");
      fflush(stdout);
      return 3;
    }
    // printf("%s\n", alias);
    deleteAlias(&ll0, alias);
    return 3;
  }
  else if (strcmp(token ,">") == 0) {
    strcpy(output, "NOOUTPUTFOUND");
    return 1;
  }  
  
  /* walk through other tokens */
  while( token != NULL ) {
    // printf( " %s\n", token );
    // fflush(stdout);
    // status  = ();
    if (strcmp(token ,">") == 0) {
      // printf( " %s is found, return soon\n", token );
      // tokens[i] = token;
      i++;
      token = strtok(NULL, delim);
      if (token == NULL) {
        strcpy(output, "NOOUTPUTFOUND");
        return 1;
      } 
      strcpy(output, token);
      if (strtok(NULL, delim) != NULL) {
        // printf("SETTING!!!!!!");
        strcpy(output, "MORETHANONE");
        return 1;
      }
      tokens[i] = NULL;
      // printf( " %s\n", token );
      return 1;
    }
    tokens[i] = token;
    // printf( " %s\n", tokens[i] );
    // fflush(stdout);

    i++;
    token = strtok(NULL, delim);
  }
  tokens[i] = NULL;
  return 0;
}

void mysh_loop(void)
{
  char line[MAX+1];
  // char **args;
  // int status;

  char output[MAX];
  char **args = malloc(TOK_BUFSIZE * sizeof(char*));
  int redir_flag = 0;
  int status = 0;
  while (1) {

    printf("mysh> ");
    fflush(stdout);
    // printList(&ll0);
    status = mysh_read_line(line);
    if (status == 100) {
      freeList(&ll0);
      _exit(0);
    }
    if (line == NULL)
      continue;
    // printf("line is %s\n",line);
    // if (line == EOF)
    //   _exit(0);
    int i = 0, j = 0;
    char line_copy[MAX+2];
    memset(line_copy,'\0', MAX+2);
    // for loop to deal with redirection spacing
    for (i = 0; i<strlen(line); i++) {
      if (line[i] != '>') {
        line_copy[j] = line[i];
        j++;
      } else {
        line_copy[j] = ' ';
        j++;
        line_copy[j] = '>';
        j++;
        line_copy[j] = ' ';
        j++;
      }
    }

    redir_flag = mysh_parse_line(line_copy, args, output);
    if (redir_flag == 100) {
      free(args);
      freeList(&ll0);
      _exit(0);
    }
    // printf("%s, %s, %s\n", args[0], args[1], args[2]);
    if (redir_flag > 1)
      continue;
    if (redir_flag == 1 && (strcmp(output, "NOOUTPUTFOUND") == 0 || strcmp(output, "MORETHANONE") == 0)) {
      fprintf(stderr, "Redirection misformatted.\n");
      fflush(stderr);
      continue;        
    }

    
    /* get the first token */
    // printf("%s, %s, %s\n", args[0], args[1], args[2]);
    // printf("output is %s\n", output);
    struct Node* full = search(&ll0, args[0]);
    if (full != NULL)
      args[0] = full->path;
    // printf("After translation command is %s\n", args[0]);
    status =  mysh_execute(args, redir_flag, output);
    if (status == 100) {
      // fclose(fp);
      free(args);
      freeList(&ll0);
      _exit(1);
    }

    // args = mysh_split_line(line);
    // status = mysh_execute(args);

    // free(line);
    // printf("%s\n", line);
    
    // printf("%d\n", status);
  }
  free(args);
}

int main(int argc, char **argv)
{

  if (argc == 1) {
    // Run command loop.
    mysh_loop();
  } else if (argc == 2) {
    FILE *fp = fopen(argv[1], "r");
    if (fp == NULL) {
      fprintf(stderr, "Error: Cannot open file %s.\n", argv[1]);
      fflush(stderr);
      freeList(&ll0);
      _exit(1);
    }
    char buf[XMAX+1];
    int status;
    char output[MAX];
    memset(output,'\0', MAX+2);
    char **args = malloc(TOK_BUFSIZE * sizeof(char*));
    while (fgets(buf, XMAX, fp)) {
      if (strlen(buf) > 512) {
        printf("(%.512s)\n", buf);
        fflush(stdout);
        fprintf(stderr, "Error: line longer than 512 characters!\n");
        fflush(stderr);
        continue;
      }
      printf("%s", buf);
      fflush(stdout);
      int redir_flag = 0;
      int i = 0, j = 0;
      char line_copy[MAX+2];
      memset(line_copy,'\0', MAX+2);
      // for loop to deal with redirection spacing
      for (i = 0; i<strlen(buf); i++) {
        if (buf[i] != '>') {
          line_copy[j] = buf[i];
          j++;
        } else {
          line_copy[j] = ' ';
          j++;
          line_copy[j] = '>';
          j++;
          line_copy[j] = ' ';
          j++;
        }
      }
      redir_flag = mysh_parse_line(line_copy, args, output);
      if (redir_flag == 100) {
        fclose(fp);
        free(args);
        freeList(&ll0);
        _exit(0);
      }      
      // printf("%d, %s\n", redir_flag, output);
      if (redir_flag > 1)
        continue;

      if (strcmp(output, "NOOUTPUTFOUND") == 0 || strcmp(output, "MORETHANONE") == 0) {
        // printf("IN!!!!!!!!!!!!!!");
        fprintf(stderr, "Redirection misformatted.\n");
        fflush(stderr);
        continue;        
      }

      
      /* get the first token */
      // printf("%s, %s, %s\n", args[0], args[1], args[2]);
      // printf("output is %s\n", output);
      struct Node* full = search(&ll0, args[0]);
      if (full != NULL)
        args[0] = full->path;
      // printf("After translation command is %s\n", args[0]);
      status = mysh_execute(args, redir_flag, output);
      if (status == 100) {
        fclose(fp);
        free(args);
        freeList(&ll0);
        _exit(1);
      }
      
      // printf("%d\n", status);
    }
    free(args);
    fclose(fp);
  } else {
    char *msg = "Usage: mysh [batch-file]\n"; /* The actual message*/
    int len = strlen(msg);  /* the byte length of the string */
    write(STDERR_FILENO, msg, len);
    _exit(1);
    // printf("Usage: mysh [batch-file]\n");
  }

  // Perform any shutdown/cleanup.
  freeList(&ll0);
  return 0;
}
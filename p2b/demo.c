#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "exec_test.c"

#define MAX 512
#define delim " \n"
#define TOK_BUFSIZE 64

void *mysh_read_line(char *buf){
  fgets(buf, MAX, stdin);
}

int mysh_parse_line(char *line, char **tokens, char *output){
  int i = 0;
  char *token;
  token = strtok(line, delim);
    if (strcmp(token ,"exit") == 0)
      _exit(0);    
    
    /* walk through other tokens */
    while( token != NULL ) {
      printf( " %s\n", token );
      // status  = ();
      if (strcmp(token ,">") == 0) {
        printf( " %s is found, return soon\n", token );
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
    mysh_read_line(line);
    redir_flag = mysh_parse_line(line, args, output);
    
    /* get the first token */
    // printf("%s, %s, %s\n", args[0], args[1], args[2]);
    printf("output is %s\n", output);
    mysh_execute(args, redir_flag, output);
    // args = mysh_split_line(line);
    // status = mysh_execute(args);

    // free(line);
    // printf("%s\n", line);
    
    // printf("%d\n", status);
    // free(args);
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
    while (fgets(buf, MAX, fp)) {
      printf("%s", buf);
    }
    fclose(fp);
  } else {
    printf("Usage: mysh [batch-file]\n");
  }

  // Perform any shutdown/cleanup.

  return 0;
}
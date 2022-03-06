#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/wait.h>
#include "dup2_test.c"

int mysh_execute(char **args, int redir_flag, char *output){
	pid_t pid;
	int ret = 1;
	int status;
	pid = fork();

	if (pid == -1){
		// pid == -1 means error occured
		printf("can't fork, error occured\n");
		exit(EXIT_FAILURE);
	}
	else if (pid == 0){
		if (redir_flag == 1) {
			redirect(output);
		}

		printf("child process, pid = %u\n",getpid());
		printf("parent of child process, pid = %u\n",getppid());

		// char * argv_list[] = {"/bin/ls", "-l", ".", NULL};
		int i = 0;
		int j = 1;
		char *token;
		// printList(&ll0);
		token = strtok(args[0], " \n");
		char *tokens[64];
		
		/* walk through other tokens */
		while( token != NULL ) {
			// printf( " %s\n", token );
			tokens[i] = token;
			i++;
			token = strtok(NULL, " \n");
		}
		while (args[j] != NULL) {
			tokens[i] = args[j];
			i++;
			j++;
		}
		tokens[i] = NULL;

		ret = execv(tokens[0],tokens);
		printf("return of execv() is %d\n", ret);
		printf("%s\n", args[0]);
		exit(1);
	}
	else{
		printf("Parent Of parent process, pid = %u\n",getppid());
		printf("parent process, pid = %u\n",getpid());

		if (waitpid(pid, &status, 0) > 0) {
			
			if (WIFEXITED(status) && !WEXITSTATUS(status))
			printf("program execution successful\n");
			
			else if (WIFEXITED(status) && WEXITSTATUS(status)) {
				if (WEXITSTATUS(status) == 127) {

					// execv failed
					printf("execv failed\n");
				}
				else
					printf("program terminated normally,"
					" but returned a non-zero status\n");				
			}
			else
				printf("program didn't terminate normally\n");			
		}
		else {
			// waitpid() failed
			printf("waitpid() failed\n");
		}
	//exit(0);
	}
	return 0;
}

// int main() {
// 	mysh_execute(NULL);
// 	return 0;
// }


#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

void redirect(char *output_path)
{
	int fd = open(output_path, O_WRONLY | O_CREAT | O_TRUNC, 0644); // [1]
	dup2(fd, fileno(stdout));                                     // [2]
}

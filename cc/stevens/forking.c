#include "ourhdr.h"

static int global = 6;
static char *buffer = "a write to stdout\n";

int main(int argc, char **argv) {

	int			stack_int = 88;
	pid_t		pid;

	if (write(STDOUT_FILENO, buffer, strlen(buffer)) != strlen(buffer)) perror("write error");

	printf("Before fork(2)…\n");
	if ((pid = fork()) < 0) perror("fork error");		// Failed to fork(2)
	else if (!pid) { global++; stack_int++; }			// Child process
	else sleep(2);										// Parent process

	printf("pid = %d global = %d stack = %d\n", getpid(), global, stack_int);

	return 0;
}

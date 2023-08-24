#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>



/* execve function */

int _fnexecve(char **, char **);


/**
 * main : simple shell
 *
 * Return : Always 0
 */

int main(int argc, char **argv, char **env)
{
	char *line = NULL;
	size_t line_size = 0;
	ssize_t read;
	char *nwline = "$ ";
	pid_t child_pid;
	int status;
	bool one_use = false;


	if (argc > 1 && argv[1] != NULL)
		return(1);

	while (1 && !one_use)
	{

		/* check if the arg is from pipe or not */
		if (isatty(STDIN_FILENO) == 0)
			one_use = true;

		/* print $ sign to the terminal */
		write(STDOUT_FILENO, nwline, 2);

		/* read data from the std input */
		read = getline(&line, &line_size, stdin);
		if (read == -1)
		{
			perror("Error in getline");
			free(line);
			return (1);
		}

		if (line[read - 1] == '\n')
			line[read - 1] = '\0';

		/* creating a child process to execute the command */
		child_pid = fork();
		if (child_pid == -1)
		{
			perror("Error: fork");
			return (1);
		}
		/* child process to execute the commande */
		if (child_pid == 0)
		{
			_fnexecve(&line, env);
		}

		if (waitpid(child_pid, &status, 0) == -1)
		{
			perror("Error waiting for the child");
			return (1);
		}
	}

	free(line);
	return (0);
}

/* call execve function */

int _fnexecve(char **args, char **envp)
{
	/*char *argv;

	argv = strtok(args, " ");*/

	if (execve(args[0], args, envp) == -1)
	{
		perror("Error execve");
		return (1);
	}

	return (0);
}

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

#define MAX_TOKENS 64

/* execve function */

int _fnexecve(char **);

/* check file */
int fileExists(const char *filename) {
	struct stat buffer;
	return (stat(filename, &buffer) == 0);
}

/**
 * main : simple shell
 *
 * Return : Always 0
 */

int main(void)
{
	char *line = NULL;
	size_t line_size = 0;
	ssize_t read;
	char *nwline = "$ ";
	pid_t child_pid;
	int status;
	bool one_use = false;
	char *tokens[MAX_TOKENS];
	int token_count = 0;
	char *token = NULL;

	while (1 && !one_use)
	{

		/* check if the arg is from pipe or not */
		if (isatty(STDIN_FILENO) == 0)
			one_use = true;
		else
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
			/* Tokenize the line using strtok */
			token = strtok(line, " ");
			while (token != NULL && token_count < MAX_TOKENS - 1)
			{
				tokens[token_count] = token;
				token_count++;
				token = strtok(NULL, " ");
			}


			tokens[token_count] = NULL;

			if (fileExists(tokens[0]))
			{
				_fnexecve(tokens);
			}
			else
			{
				perror("");
				return (1);
			}
		}

		if (waitpid(child_pid, &status, 0) < 0 && errno != ECHILD)
		{
			perror("Error waiting for the child");
			return (1);
		}
	}

	free(line);
	return (0);
}

/* call execve function */

int _fnexecve(char **tokens)
{

	if (execve(tokens[0], tokens, NULL) == -1)
	{
		perror("Error execve");
		return (1);
	}

	return (0);
}

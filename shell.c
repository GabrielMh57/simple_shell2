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

int _fnexecve(char **, char **);

/* check file */
int fileExists(const char *filename)
{
	struct stat buffer;

	return (stat(filename, &buffer) == 0);
}

/**
 * main : simple shell
 *
 * Return : Always 0
 */

int main(int argc, char *argv[], char *envp[])
{
	char *line = NULL;
	size_t line_size = 0;
	ssize_t read;
	char *nwline = "#shell$ ";
	pid_t child_pid;
	int status;
	bool one_use = false;
	char *tokens[MAX_TOKENS];
	int token_count = 0;
	char *token = NULL;
	int count_arg = 0;

	while (argv[0][count_arg] != '\0')
		count_arg++;

	while (1 && !one_use)
	{
		if (argc > 3)
			return (1);
		/* check if the arg is from pipe or not */
		if (isatty(STDIN_FILENO) == 0)
			one_use = true;
		else
			/* print $ sign to the terminal */
			write(STDOUT_FILENO, nwline, 8);

		/* read data from the std input */
		if ((read = getline(&line, &line_size, stdin)) == -1)
		{

			perror("Error in getline");
			free(line);

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
				_fnexecve(tokens, envp);
			}
			else
			{
				write(STDOUT_FILENO, argv[0], count_arg);
				perror(" ");
				exit (1);
			}
		}

		if (waitpid(child_pid, &status, 0) < 0 && errno != ECHILD)
		{
			perror("Error waiting for the child");
		}
	}
	

	free(line);
	return (0);
}

/* call execve function */

int _fnexecve(char **tokens, char **envp)
{

	if (execve(tokens[0], tokens, envp) == -1)
	{
		perror("Error execve");
		return (1);
	}

	return (0);
}

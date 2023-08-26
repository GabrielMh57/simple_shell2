#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include "path.h"

#define MAX_TOKENS 64

/* execve function */

int _fnexecve(char **, char **);

/**
 * fileExists - check file stat
 *
 * Description : checking file
 * @filename : file name
 *
 *Return: 0 (Success)  or  1
 */
int fileExists(const char *filename)
{
	struct stat buffer;

	return (stat(filename, &buffer) == 0);
}

/**
 * main - simple shell
 *
 * Description : simple shell
 * @argc : nmbr of arguments
 * @argv : value of arguments
 * @envp : enviroment variable
 *
 *Return: Always 0 (Success)
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
	char path[] = "/bin:/usr/bin";
	char *executable = NULL;

	while (argv[0][count_arg] != '\0')
		count_arg++;

	while (1 && !one_use)
	{
		token_count = 0;
		if (argc > 3)
			return (1);


		/* check if the arg is from pipe or not */
		if (isatty(STDIN_FILENO) == 0)
			one_use = true;
		else
			/* print $ sign to the terminal */
			write(STDOUT_FILENO, nwline, 8);

		/* read data from the std input */
		read = getline(&line, &line_size, stdin);
		if (read == -1)
		{
			if (read == EOF)
				break;

			perror("Error in getline");
			free(line);
			exit(1);
		}

		if (read == 1 && line[0] == '\n')
		{
			/* Empty line, prompt again */
			write(STDOUT_FILENO, "enter cmd\n", 10);
			continue;
		}

		if (line[read - 1] == '\n')
			line[read - 1] = '\0';

		/* Tokenize the line using strtok */
		token = strtok(line, " ");
		while (token != NULL && token_count < MAX_TOKENS - 1)
		{
			tokens[token_count] = token;
			token_count++;
			token = strtok(NULL, " ");
		}


		tokens[token_count] = NULL;

		/* check if file do not exists to check path*/
		if (!fileExists(tokens[0]))
		{
			executable = findExecutable(tokens[0], path);
			if (executable == NULL) {
				write(STDOUT_FILENO, argv[0], count_arg);
				perror(" ");
				continue;
			}

			tokens[0] = executable;
			/*if (copyExecutable(tokens,executable) == 1)
			{
				perror("Memory alloc");
				exit(1);
			}
			*/
		}

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

			if (fileExists(tokens[0]))
			{
				_fnexecve(tokens, envp);
			}
			else
			{
				write(STDOUT_FILENO, argv[0], count_arg);
				perror(" ");
				exit(1);
			}
		}

		if (waitpid(child_pid, &status, 0) < 0 && errno != ECHILD)
		{
			perror("Error waiting for the child");
		}

	}

	free(line);

	/* return to end program if success*/
	return (0);
}

/**
 * _fnexecve - call execve function
 *
 * Description : exec commands
 * @tokens : command plus args
 * @envp : enviroment variable
 *
 *Return: 0 upon success otherwise 1
 */

int _fnexecve(char **tokens, char **envp)
{

	if (execve(tokens[0], tokens, envp) == -1)
	{
		perror("Error execve");
		return (1);
	}

	return (0);
}

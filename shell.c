#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>




int main(int ac, char **av, char **env)
{
	char *line = NULL;
	size_t line_size = 0;
	ssize_t read;
	char *nwline = "$ ";
	pid_t child_pid;

	while (1)
	{
		/* print $ sign to the terminal */
		write(STDOUT_FILENO, nwline, 2);

		/* read data from the std input */
		read = getline(&line, &line_size, stdin);
		if (read == -1)
		{
			perror("Error in getline");
			free(line);
			exit(EXIT_FAILURE);
		}

		if (line[read - 1] == '\n')
			line[read - 1] = '\0';

		/* creating a child process to execute the command */
		child_pid = fork();

	}

	free(line);
}

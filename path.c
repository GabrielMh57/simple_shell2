#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

size_t getLength(const char *str) {
	size_t length = 0;
	while (str[length] != '\0') {
		length++;
	}
	return length;
}


/**
 * copyExecutable - Malloc memory and copy executable path
 *
 * Description : Malloc memory for the executable path and copy it to tokens array
 * @tokens : tokens array
 * @executable : executable path
 *
 * Return: 0 upon success, otherwise 1
 */
int copyExecutable(char **tokens, const char *executable)
{
    size_t i;
    size_t len = getLength(executable);
    tokens[0] = malloc(len + 1);
    if (tokens[0] == NULL) {
        perror("Error allocating memory");
        return 1;
    }

    for (i = 0; i <= len; i++) {
        tokens[0][i] = executable[i];
    }

    return 0;
}

char *findExecutable(char *command, char *path) {

    char *dir = NULL;
    size_t i;
    size_t j = 0;
    char *executable = NULL;

    dir = strtok(path, ":");

    while (dir != NULL) {
        size_t dirLen = getLength(dir);
        size_t commandLen = getLength(command);

	/* +2 for '/' and '\0' */
        executable = (char *)malloc(dirLen + commandLen + 2);
        if (executable == NULL)
	{
            return NULL;
        }

        for (i = 0; i < dirLen; i++)
	{
            executable[i] = dir[i];
        }
        executable[i] = '/';
        i++;

        while (j < commandLen)
	{
            executable[i] = command[j];
            i++;
            j++;
        }
        executable[i] = '\0';

        if (access(executable, X_OK) == 0) 
	{
	      return executable;
        }

        free(executable);
        dir = strtok(NULL, ":");
    }
    /* if not found */
    return NULL;
}


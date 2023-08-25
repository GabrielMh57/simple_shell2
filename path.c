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

char *findExecutable(char *command, char *path) {
    char *dir = strtok(path, ":");
    size_t i;
    size_t j = 0;

    while (dir != NULL) {
        size_t dirLen = getLength(dir);
        size_t commandLen = getLength(command);

	/* +2 for '/' and '\0' */
        char *executable = (char *)malloc(dirLen + commandLen + 2);
        if (executable == NULL) {
            return NULL;
        }

        for (i = 0; i < dirLen; i++) {
            executable[i] = dir[i];
        }
        executable[i] = '/';
        i++;

        while (j < commandLen) {
            executable[i] = command[j];
            i++;
            j++;
        }
        executable[i] = '\0';

        if (access(executable, X_OK) == 0) {
            return executable;
        }

        free(executable);
        dir = strtok(NULL, ":");
    }
    return NULL; // Not found
}


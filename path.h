
#ifndef PATH_H
#define PATH_H

size_t getLength(const char *str);
int copyExecutable(char **tokens, const char *executable);
char *findExecutable(char *command, char *path);

#endif


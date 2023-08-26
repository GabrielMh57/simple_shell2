#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "compare.h"

/**
 * compareExit - Compare tokens[0] with "exit"
 *
 * Description: Compare the first token with "exit"
 * @tokens: Array of tokens
 *
 * Return: true if tokens[0] matches "exit", false otherwise
 */
bool compareExit(char **tokens) {
    const char *exitStr = "exit";
    int i = 0;

    while (tokens[0][i] != '\0' && exitStr[i] != '\0') {
        if (tokens[0][i] != exitStr[i]) {
            return false;
        }
        i++;
    }

    return tokens[0][i] == '\0' && exitStr[i] == '\0';
}

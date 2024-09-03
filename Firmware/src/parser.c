#include <string.h>
#include "parser.h"

void parseCommand(char *input, Command_t *cmd)
{
    char *token;
    int argIndex = 0;

    // Copy the input command to the command structure
    strncpy(cmd->command, input, MAX_COMMAND_LENGTH);

    // Tokenize the input string
    token = strtok(cmd->command, " ");
    while (token != NULL && argIndex < MAX_ARGS)
    {
        cmd->argv[argIndex] = token;
        argIndex++;
        token = strtok(NULL, " ");
    }

    // Set the argument count
    cmd->argc = argIndex;
}

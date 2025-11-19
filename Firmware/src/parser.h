#ifndef parser_h
#define parser_h

#include "FreeRTOS.h"
#include "console.h"

void parseCommand(char *input, Command_t *cmd);

#endif
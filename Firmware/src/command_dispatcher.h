#ifndef COMMAND_DISPATCHER_H
#define COMMAND_DISPATCHER_H

/* Standard C Library */
#include <stdio.h>
#include <string.h>

/* FreeRTOS */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

/* Local Includes */
#include "local_queues.h"
#include "parser.h"
#include "led_task.h"

/* Function pointer type for command handlers */
typedef void (*CommandHandler_t)(Command_t *);

/* Structure to map command strings to handler functions */
typedef struct
{
    const char *command;
    CommandHandler_t handler;
} CommandMapping_t;

/* Function Declarations */
void commandDispatcherTask(void *pvParameters);
void processCommand(Command_t *cmd);
void handleVersionCommand(Command_t *cmd);
void handleHelpCommand(Command_t *cmd);
void handleUnknownCommand(Command_t *cmd);

#endif // COMMAND_DISPATCHER_H

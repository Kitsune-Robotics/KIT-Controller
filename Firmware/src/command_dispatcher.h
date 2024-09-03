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

/* Function Declarations */
void commandDispatcherTask(void *pvParameters);
void processCommand(Command_t *cmd);

#endif // COMMAND_DISPATCHER_H

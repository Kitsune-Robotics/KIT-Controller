#ifndef CAN_CONTROLLER_H
#define CAN_CONTROLLER_H

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "command_dispatcher.h"

enum CANCommands
{
    CAN_COMMAND_GET_VERSION
};

// Function to initialize and start the CAN task
void can_task(void *pvParams);

// Function to request version info from the MCP2515
void request_can_version(Command_t *cmd);

#endif // CAN_CONTROLLER_H

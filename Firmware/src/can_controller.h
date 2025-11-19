#ifndef CAN_CONTROLLER_H
#define CAN_CONTROLLER_H

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "command_dispatcher.h"

enum CANCommands
{
    CAN_COMMAND_GET_STATUS,
    CAN_COMMAND_SEND_FRAME
};

// Function to initialize and start the CAN task
void can_task(void *pvParams);

// Function to request version info from the MCP2515
void request_can_status(Command_t *cmd);

// Send a test frame via canbus
void send_can_frame(Command_t *cmd);

#endif // CAN_CONTROLLER_H

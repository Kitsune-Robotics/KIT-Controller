#ifndef CAN_CONTROLLER_H
#define CAN_CONTROLLER_H

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "mcp2515/mcp2515.h"

// Function to initialize and start the CAN task
void can_task(void *pvParams);

// Function to request version info from the MCP2515
void request_can_version(void);

#endif // CAN_CONTROLLER_H

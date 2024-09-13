#include "can_controller.h"
#include "pico/stdlib.h"
#include "console.h"
#include "mcp2515.h"
#include "can.h"

const uint8_t CAN_QUEUE_LENGTH = 10;

static QueueHandle_t canQueue; // RTOS queue for CAN commands
static MCP2515 can0;           // MCP2515 CAN controller instance
struct can_frame rx;

// Function to request version info from the MCP2515
void request_can_version(Command_t *cmd)
{
    if (canQueue == NULL)
    {
        // Ensure the queue is initialized before using it
        return;
    }

    // Send a version request command to the CAN task
    uint8_t versionCommand = CAN_COMMAND_GET_VERSION;
    xQueueSend(canQueue, &versionCommand, 0);
}

// Function to initialize CAN communication
void setup_can()
{
    // Initialize interface
    can0.reset();
    can0.setBitrate(CAN_1000KBPS, MCP_16MHZ);
    can0.setNormalMode();
}

// The CAN task function
void can_task(void *pvParams)
{
    uint8_t canCommand;                                // The general command to observe
    TickType_t lastWakeTime;                           // The last time we got a general command that wasn't idle
    const TickType_t tickInterval = pdMS_TO_TICKS(10); // 10 ms interval for checking commands

    // Setup CAN communication
    setup_can();

    // Create the CAN command queue
    canQueue = xQueueCreate(CAN_QUEUE_LENGTH, sizeof(uint8_t));
    if (canQueue == NULL)
    {
        // Handle error (e.g., print an error message)
        console_printf(BROADCAST, "Failed to create CAN command queue.\n");
        vTaskDelete(NULL); // Delete the task if the queue creation fails
    }

    // Initialize the last wake time for accurate timing
    lastWakeTime = xTaskGetTickCount();

    while (true)
    {
        // Check for new commands in the queue
        if (xQueueReceive(canQueue, &canCommand, 0) == pdPASS)
        {
            // Handle the received command
            if (canCommand == CAN_COMMAND_GET_VERSION)
            {
                // Retrieve the MCP2515 version
                uint8_t version = can0.getStatus();
                console_printf(BROADCAST, "MCP2515 Version: 0x%x\n", version);
            }
        }

        // Wait for the next cycle with a short interval to remain responsive
        vTaskDelayUntil(&lastWakeTime, tickInterval);
    }
}

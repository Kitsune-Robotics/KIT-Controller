#include "command_dispatcher.h"

void processCommand(Command_t *cmd)
{
    // Example command processing function
    if (cmd == NULL)
    {
        console_printf(BROADCAST, "Received a NULL command, nothing to process.\n");
        return;
    }

    // console_printf(cmd->console, "Processing command: %s\n", cmd->command);

    // Implement command-specific logic
    if (strcmp(cmd->command, "led_on") == 0)
    {
        console_printf(cmd->console, "Turning LED on...\n");
        // gpio_put(PIN_ONBOARD_LED, 1); // Assuming you have an LED control command
    }
    if (strcmp(cmd->command, "version") == 0)
    {
        console_printf(cmd->console, "SDK version %s\n", PICO_SDK_VERSION_STRING);
        // gpio_put(PIN_ONBOARD_LED, 1); // Assuming you have an LED control command
    }
    else if (strcmp(cmd->command, "led_off") == 0)
    {
        console_printf(cmd->console, "Turning LED off...\n");
        // gpio_put(PIN_ONBOARD_LED, 0);
    }
    else
    {
        console_printf(cmd->console, "Unknown command: %s\n", cmd->command);
    }

    // Add more command-specific handling as needed
}

void commandDispatcherTask(void *pvParameters)
{
    Command_t *cmd; // Pointer to hold dequeued commands

    for (;;)
    {
        // Wait indefinitely for a command to become available in the queue
        if (xQueueReceive(cmdQueue, &cmd, portMAX_DELAY) == pdPASS)
        {
            // Process the command
            processCommand(cmd);

            // After processing, free the allocated memory for the command
            vPortFree(cmd);
        }
        else
        {
            console_printf(cmd->console, "Failed to receive a command from the queue.\n");
        }
    }
}

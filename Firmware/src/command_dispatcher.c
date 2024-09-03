#include "command_dispatcher.h"

// Define the command mapping table
const CommandMapping_t commandTable[] = {
    {"version", handleVersionCommand}, // Prints the version
    {"help", handleHelpCommand},       // Prints the help
    {NULL, handleUnknownCommand}       // Default handler for unknown commands
};

// Simple retrieve version
void handleVersionCommand(Command_t *cmd)
{
    console_printf(cmd->console, "Firmware version: %s\n", "1.2.3.4");
}

// Handling an unknown command
void handleUnknownCommand(Command_t *cmd)
{
    console_printf(cmd->console, "Unknown command: %s\n", cmd->command);
}

// Handling the help command
void handleHelpCommand(Command_t *cmd)
{
    console_printf(cmd->console, "Available commands:\n");

    // Iterate over the commandTable and print each command
    for (int i = 0; commandTable[i].command != NULL; i++)
    {
        console_printf(cmd->console, "  %s\n", commandTable[i].command);
    }
}

void processCommand(Command_t *cmd)
{
    if (cmd == NULL)
    {
        console_printf(USB, "Received a NULL command, nothing to process.\n");
        return;
    }

    // console_printf(cmd->console, "Processing command: %s\n", cmd->command);

    // Iterate through the command table to find the matching handler
    for (int i = 0; commandTable[i].command != NULL; i++)
    {
        if (strcmp(cmd->command, commandTable[i].command) == 0)
        {
            // Call the associated handler function
            commandTable[i].handler(cmd);
            return;
        }
    }

    // If no matching command is found, call the unknown command handler
    handleUnknownCommand(cmd);
}

void commandDispatcherTask(void *pvParameters)
{
    Command_t *cmd; // Pointer to hold dequeued commands

    for (;;)
    {
        // Wait indefinitely for a command to become available in the queue
        if (xQueueReceive(cmdQueue, &cmd, portMAX_DELAY) == pdPASS)
        {
            // Process the command using the enhanced dispatcher logic
            processCommand(cmd);

            // After processing, free the allocated memory for the command
            vPortFree(cmd);
        }
        else
        {
            console_printf(BROADCAST, "Failed to receive a command from the queue.\n");
        }
    }
}

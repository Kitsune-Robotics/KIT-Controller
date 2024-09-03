/* Raspberry Pi */
#include <stdio.h>
#include "pico/stdlib.h"

/* FreeRTOS */
#include "FreeRTOS.h"
#include "task.h"

// Local
#include "local_queues.h"
#include "usb_console.h"
#include "parser.h"

void usb_console(void *pvParams)
{
    /* Hopefully a simple design strategy.
     *
     * This function should be able to read in lines of data from the serial
     * interface, parse them and add them to the command queue for processing.
     *
     * The code quality might be a little subpar but its only really going
     * to be used as a quick way to get commands into the parser,
     * there wont be a USB serial interface like this used on the actual robot.
     */

    char inputBuffer[MAX_COMMAND_LENGTH]; // Buffer to store commands as they're typed
    uint bufIndex = 0;                    // Index along the inputBuffer
    int ch;                               // Must be type int because errors are returned as negative

    stdio_flush(); // Ensure the serial output is flushed

    while (true)
    {
        ch = getchar_timeout_us(0); // Non-blocking read from serial

        // Handle timeout (no input received yet)
        if (ch == PICO_ERROR_TIMEOUT)
        {
            vTaskDelay(100 / portTICK_PERIOD_MS); // Delay before checking again
            continue;
        }

        printf("%c", ch); // Echo the character back to the console

        if (!(ch == '\n' || ch == '\r')) // Not end-of-line, continue buffering input
        {
            if (bufIndex < MAX_COMMAND_LENGTH - 1) // Prevent buffer overflow
            {
                inputBuffer[bufIndex++] = ch; // Add character to buffer
            }
        }
        else // End of line received
        {
            inputBuffer[bufIndex] = '\0'; // Null-terminate the string

            // Allocate memory for a new command
            Command_t *cmd = (Command_t *)pvPortMalloc(sizeof(Command_t));
            if (cmd != NULL) // Ensure memory allocation succeeded
            {
                parseCommand(inputBuffer, cmd); // Parse input into command structure

                // This is the USB console
                cmd->console = USB;

                console_printf(USB, "\n Sending Command: %s\n with %d args\n", cmd->command, cmd->argc);

                // Check if the queue is initialized
                if (cmdQueue == NULL)
                {
                    console_printf(USB, "Error! cmdQueue not ready or uninitialized.\n");
                    vPortFree(cmd); // Free allocated memory if queue is not ready
                    continue;
                }

                // Attempt to queue the command
                if (xQueueSend(cmdQueue, &cmd, 0U) != pdPASS)
                {
                    // Queue is full or sending failed
                    console_printf(USB, "Could not queue cmd. Queue might be full.\n");
                    vPortFree(cmd); // Free allocated memory if not added to the queue
                }

                // No need to free cmd here; it should be managed by the task processing the queue
            }
            else
            {
                console_printf(USB, "Memory allocation failed for Command_t!\n");
            }

            bufIndex = 0; // Reset buffer index for the next command
        }
    }
}

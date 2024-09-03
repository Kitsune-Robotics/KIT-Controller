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

    char inputBuffer[MAX_COMMAND_LENGTH]; // Buffer to store commands as they're being typed
    uint bufIndex = 0;                    // Index along the inputBuffer
    int ch;                               // Must be type int because errors are returned as negative

    stdio_flush();

    while (true)
    {
        ch = getchar_timeout_us(0);

        // Buffer not full, wait and check again
        if (ch == PICO_ERROR_TIMEOUT)
        {
            vTaskDelay(100 / portTICK_PERIOD_MS);
            continue;
        }

        printf("%c", ch); // Echo the character

        if (!(ch == '\n' || ch == '\r'))           // If we haven't reached the end of the line
        {                                          //
            if (bufIndex < MAX_COMMAND_LENGTH - 1) // And we're not trying to go over the buffer
            {                                      //
                inputBuffer[bufIndex++] = ch;      // Add character to buffer
            }
        }
        else                              // We have reached the end of this line
        {                                 //
            inputBuffer[bufIndex] = '\0'; // Null-terminate the string

            // Allocate memory for a new command
            uint cmd = 3;
            // pvPortMalloc();
            if (true) // (placeholder, check if malloc allocated memory or not)
            {
                // parseCommand(inputBuffer, cmd);
                // printf("\n [~] %s\n", cmd->command);

                // printf("%d\n", cmd->argc);
                // printf("%d\n", cmd->console);
                // cmd->intA = 15;
                // cmd->intB = 2;

                // // Add the command to the queue
                if (cmdQueue == NULL)
                {
                    printf("Error! cmdQueue not ready or uninitialized.\n");
                    continue;
                }

                if (xQueueSend(cmdQueue, &cmd, 0U) != pdPASS)
                {
                    // Handle the error (e.g., queue is full)
                    printf("Could not queue cmd.\n");
                }

                // free(cmd);
            }

            bufIndex = 0; // Reset buffer index for the next command
        }
    }
}

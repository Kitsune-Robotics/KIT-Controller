/* Raspberry Pi */
#include <stdio.h>
#include "pico/stdlib.h"
#include "malloc.h"

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

    char inputBuffer[MAX_COMMAND_LENGTH];
    int bufIndex = 0;
    int ch;

    stdio_flush();

    while (true)
    {
        ch = getchar_timeout_us(0);

        if (ch != PICO_ERROR_TIMEOUT)
        {
            printf("%c", ch);                     // Echo the character
            if (ch == '\n' || ch == '\r')         // If you read this char.. you're done!
            {                                     //
                if (bufIndex > 0)                 // Double check that the bufIndex has moved at least 1 char
                {                                 //
                    inputBuffer[bufIndex] = '\0'; // Null-terminate the string

                    // Allocate memory for a new command
                    Command_t *cmd = (Command_t *)malloc(sizeof(Command_t));
                    if (cmd != NULL)
                    {
                        parseCommand(inputBuffer, cmd);
                        printf("\n [~] %s\n", cmd->command);

                        printf("%d\n", cmd->argc);
                        printf("%d\n", cmd->console);

                        // // Add the command to the queue
                        if (xQueueSend(cmdQueue, &cmd, 200) != pdPASS)
                        {
                            // Handle the error (e.g., queue is full)
                            printf("Could not queue cmd.\n");
                        }

                        free(cmd);
                    }

                    bufIndex = 0; // Reset buffer index for the next command
                }
            }
            else if (bufIndex < MAX_COMMAND_LENGTH - 1)
            {
                inputBuffer[bufIndex++] = ch; // Add character to buffer
            }
        }
        else
        {
            vTaskDelay(100 / portTICK_PERIOD_MS); // Delay to allow the buffer to fill
        }
    }
}
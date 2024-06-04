/* Raspberry Pi */
#include <stdio.h>
#include "pico/stdlib.h"

/* FreeRTOS */
#include "FreeRTOS.h"
#include "task.h"

// Local
#include "usb_console.h"

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

    // Setup variables we'll need
    int _buf;

    stdio_flush();

    while (true)
    {
        // Read and echo each character

        // check if the line is complete
        // if it is, add a formatted Command_t to the cmdQueue

        _buf = getchar_timeout_us(0);

        if (_buf != PICO_ERROR_TIMEOUT)
        {
            printf("%c", _buf);
        }
        else
        {
            // I've put the delay here so if we read a long string of chars quickly (like copy/paste)
            // we evacuate the buffer as fast as we can
            vTaskDelay(100);
        }
    }
}
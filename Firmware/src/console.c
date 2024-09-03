#include <stdio.h>
#include <stdarg.h>
#include "console.h"

// Function to handle USB console output
void usb_console_output(const char *message)
{
    printf("[USB] %s", message); // USB console simply uses printf
}

// Function to handle RADIO console output
void radio_console_output(const char *message)
{
    // Implement radio output logic here
    // Example: send via UART or another communication interface
    printf("[RADIO] %s", message); // Placeholder implementation
}

// Function to handle BROADCAST console output
void broadcast_console_output(const char *message)
{
    // Broadcast will send the message to everyone at once. Useful
    // For sending global errors or issues
    printf("[BROADCAST] %s", message); // Placeholder implementation
}

// Centralized printf function to dispatch messages to the appropriate console
void console_printf(enum Console console, const char *format, ...)
{
    char buffer[256]; // Buffer to store formatted string
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args); // Format the string
    va_end(args);

    // Dispatch the formatted string to the appropriate console
    switch (console)
    {
    case USB:
        usb_console_output(buffer);
        break;
    case RADIO:
        radio_console_output(buffer);
        break;
    case BROADCAST:
        broadcast_console_output(buffer);
        break;
    default:
        printf("Unknown console type: %d\n", console);
        break;
    }
}

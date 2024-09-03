#ifndef console_h
#define console_h

#define MAX_COMMAND_LENGTH 256
#define MAX_ARGS 10

enum Console
{
    USB,
    RADIO,
    BROADCAST,
};

typedef struct
{
    char command[MAX_COMMAND_LENGTH]; // Full command string
    char *argv[MAX_ARGS];             // Pointers to arguments
    int argc;                         // Argument count
    enum Console console;             // What console to send the return to
} Command_t;

// Declare the centralized print function
void console_printf(enum Console console, const char *format, ...);

#endif

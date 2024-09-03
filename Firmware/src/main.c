/* Raspberry Pi */
#include <stdio.h>
#include "pico/stdlib.h"

/* FreeRTOS */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

/* Git Tracking */
#include "git.h"

// Our own
#include "pindefs.h"
#include "local_queues.h"
#include "console.h"            // Parent data for all consoles
#include "usb_console.h"        // USB serial console
#include "command_dispatcher.h" // Functions for dispatching commands
#include "led_task.h"           // Manages LED functions
#include "can_controller.h"     // Manages the canbus

// Global Definitions
QueueHandle_t cmdQueue = NULL;

int main()
{
    // From pico SDK, init all
    stdio_init_all();

    // FreeRTOS Create queues
    cmdQueue = xQueueCreate(4, sizeof(uint));

    // Check if the queue was created successfully
    if (cmdQueue == NULL)
    {
        // Handle error
        printf("There was an error creating the cmdQueue");
    }

    // FreeRTOS Create tasks
    xTaskCreate(led_task, "LED_Task", 256, NULL, 1, NULL);
    xTaskCreate(can_task, "CAN_Task", 512, NULL, 1, NULL);
    xTaskCreate(commandDispatcherTask, "CmdDispatcher", 512, NULL, 1, NULL);
    xTaskCreate(usb_console, "USB_Console", 1024, NULL, 1, NULL);

    // Begin scheduler
    vTaskStartScheduler();

    while (true)
    {
        // Main task can never exit
    };
}
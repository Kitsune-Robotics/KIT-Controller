/* Raspberry Pi */
#include <stdio.h>
#include "pico/stdlib.h"
#include "malloc.h"

/* FreeRTOS */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

/* Git Tracking */
#include "git.h"

// Our own
#include "pindefs.h"
#include "local_queues.h"
#include "console.h"     // Parent data for all consoles
#include "usb_console.h" // USB serial console

void led_task(void *pvParams)
{
    gpio_init(PIN_ONBOARD_LED);
    gpio_set_dir(PIN_ONBOARD_LED, GPIO_OUT);
    while (true)
    {
        gpio_put(PIN_ONBOARD_LED, 1);
        vTaskDelay(100);
        gpio_put(PIN_ONBOARD_LED, 0);
        vTaskDelay(100);
    }
}

void processCommand(Command_t *cmd);

void commandDispatcherTask(void *pvParameters)
{
    Command_t *cmd;

    for (;;)
    {
        // Technically portMAX_DELAY is not... forever, its like a few months but...
        if (xQueueReceive(cmdQueue, &cmd, portMAX_DELAY) == pdPASS)
        {
            // Process the command
            processCommand(cmd);

            // Free the allocated memory for the command
            free(cmd);
        }
    }
}

void processCommand(Command_t *cmd)
{
    // Example command processing
    if (strcmp(cmd->argv[0], "cmd") == 0)
    {
        // Handle the command
        printf("Command: %s\n", cmd->argv[0]);
        for (int i = 1; i < cmd->argc; i++)
        {
            printf("Arg %d: %s\n", i, cmd->argv[i]);
        }
    }
}

int main()
{
    // From pico SDK, init all
    stdio_init_all();

    // FreeRTOS Create queues
    cmdQueue = xQueueCreate(4, sizeof(Command_t));

    // Check if the queue was created successfully
    if (cmdQueue == NULL)
    {
        // Handle error
        ;
    }

    Command_t testcmd;

    // FreeRTOS Create tasks
    xTaskCreate(led_task, "LED_Task", 256, NULL, 1, NULL);
    xTaskCreate(commandDispatcherTask, "CmdDispatcher", 512, NULL, 1, NULL);
    xTaskCreate(usb_console, "USB_Console", 1024, NULL, 1, NULL);

    // Begin scheduler
    vTaskStartScheduler();

    while (true)
    {
        // Main task can never exit
    };
}
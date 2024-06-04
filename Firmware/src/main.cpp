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

int main()
{
    // From pico SDK, init all
    stdio_init_all();

    // FreeRTOS Create queues
    cmdQueue = xQueueCreate(2, sizeof(Command_t *));

    // FreeRTOS Create tasks
    xTaskCreate(led_task, "LED_Task", 256, NULL, 1, NULL);
    xTaskCreate(usb_console, "USB_Console", 512, NULL, 1, NULL);

    // Begin scheduler
    vTaskStartScheduler();

    while (true)
    {
        // Main task can never exit
    };
}
/* Raspberry Pi */
#include <stdio.h>
#include "pico/stdlib.h"

/* FreeRTOS */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

// Our own
#include "console.h"

// Queues
static QueueHandle_t cmdQueue = NULL;

void led_task(void *pvParams)
{
    const uint LED_PIN = 13;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    while (true)
    {
        gpio_put(LED_PIN, 1);
        vTaskDelay(100);
        gpio_put(LED_PIN, 0);
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
    xTaskCreate(usb_console, "USB_Console", 256, NULL, 1, NULL);

    // Begin scheduler
    vTaskStartScheduler();

    while (true)
    {
        // Main task can never exit
    };
}
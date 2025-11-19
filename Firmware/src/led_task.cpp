#include "led_task.h"
#include "pico/stdlib.h"

// LED control queue
#define LED_QUEUE_LENGTH 1
#define LED_COMMAND_FLASH 1

static QueueHandle_t ledQueue; // RTOS queue for led stuff
static int busyFlag = 0;       // Flag when we're busy

// Function to indicate activity (fast flash)
void busy(void)
{
    if (ledQueue == NULL)
    {
        // Ensure the queue is initialized before using it
        return;
    }

    // Send a flash command to the LED task
    uint8_t flashCommand = LED_COMMAND_FLASH;
    xQueueSend(ledQueue, &flashCommand, 0);
}

/**
 * @brief LED Controller Task
 *
 * Is it a bit overcomplicated for a flashing light? yes
 * But! It helps me learn freertos concepts, and i like UI so, hah
 */
void led_task(void *pvParams)
{
    uint8_t ledCommand;                                // The general command to observe
    TickType_t lastWakeTime;                           // The last time we got a general command that wasn't idle
    const TickType_t tickInterval = pdMS_TO_TICKS(10); // 10 ms interval for checking commands
    TickType_t slowFlashDelay = pdMS_TO_TICKS(1000);   // Slow flash delay
    TickType_t fastFlashDelay = pdMS_TO_TICKS(50);     // Fast flash delay

    // Initialize the onboard LED
    gpio_init(PIN_ONBOARD_LED);
    gpio_set_dir(PIN_ONBOARD_LED, GPIO_OUT);

    // Create the LED command queue
    ledQueue = xQueueCreate(LED_QUEUE_LENGTH, sizeof(uint8_t));
    if (ledQueue == NULL)
    {
        // Handle error (e.g., print an error message)
        console_printf(BROADCAST, "Failed to create LED command queue.\n");
        vTaskDelete(NULL); // Delete the task if the queue creation fails
    }

    // Initialize the last wake time for accurate timing
    lastWakeTime = xTaskGetTickCount();

    // State variable for LED blinking
    int ledState = 0;

    while (true)
    {
        // Check for new commands in the queue
        if (xQueueReceive(ledQueue, &ledCommand, 0) == pdPASS)
        {
            // Handle the received command
            if (ledCommand == LED_COMMAND_FLASH)
            {
                // Set the busy flag to indicate a fast flash is needed
                busyFlag = 1;
            }
        }

        if (busyFlag)
        {
            // Perform a fast flash for busy state
            gpio_put(PIN_ONBOARD_LED, 1);
            vTaskDelay(fastFlashDelay);
            gpio_put(PIN_ONBOARD_LED, 0);
            vTaskDelay(fastFlashDelay);

            // Reset the busy flag after flashing
            busyFlag = 0;
        }
        else
        {
            // Perform the regular slow flash pattern
            if (ledState == 0)
            {
                gpio_put(PIN_ONBOARD_LED, 1);
                ledState = 1;
            }
            else
            {
                gpio_put(PIN_ONBOARD_LED, 0);
                ledState = 0;
            }

            // Delay for the slow flash but check for commands in shorter intervals
            TickType_t elapsedTime = 0;
            while (elapsedTime < slowFlashDelay)
            {
                vTaskDelayUntil(&lastWakeTime, tickInterval);
                elapsedTime += tickInterval;

                // Check for new commands while waiting
                if (xQueueReceive(ledQueue, &ledCommand, 0) == pdPASS)
                {
                    if (ledCommand == LED_COMMAND_FLASH)
                    {
                        busyFlag = 1;
                        break; // Exit loop to handle busy flash immediately
                    }
                }
            }
        }

        // Wait for the next cycle with a short interval to remain responsive
        vTaskDelayUntil(&lastWakeTime, tickInterval);
    }
}
#include "can_controller.h"
#include "pico/stdlib.h"

// Define SPI pins and settings for MCP2515
#define SPI_PORT spi0
#define PIN_SCK 2
#define PIN_MOSI 3
#define PIN_MISO 4
#define PIN_CS 5

// CAN control queue
#define CAN_QUEUE_LENGTH 1
#define CAN_COMMAND_GET_VERSION 1

static QueueHandle_t canQueue; // RTOS queue for CAN commands
static MCP2515 mcp2515;        // MCP2515 CAN controller instance

// Function to request version info from the MCP2515
void request_can_version(void)
{
    if (canQueue == NULL)
    {
        // Ensure the queue is initialized before using it
        return;
    }

    // Send a version request command to the CAN task
    uint8_t versionCommand = CAN_COMMAND_GET_VERSION;
    xQueueSend(canQueue, &versionCommand, 0);
}

// Initialize SPI for MCP2515 communication
void setup_spi()
{
    // Initialize SPI at 1MHz
    spi_init(SPI_PORT, 1000 * 1000);

    // Set up SPI pins
    gpio_set_function(PIN_SCK, GPIO_FUNC_SPI);
    gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);
    gpio_set_function(PIN_MISO, GPIO_FUNC_SPI);

    // Set CS pin as output
    gpio_init(PIN_CS);
    gpio_set_dir(PIN_CS, GPIO_OUT);
    gpio_put(PIN_CS, 1); // CS high to deselect the MCP2515
}

// Function to initialize CAN communication
void setup_can()
{
    // Initialize the MCP2515 driver
    mcp2515_init(&mcp2515, SPI_PORT, PIN_CS);

    // Reset the MCP2515 to enter configuration mode
    mcp2515_reset(&mcp2515);

    // Set bit rate to 500kbps
    mcp2515_set_bitrate(&mcp2515, MCP_500KBPS);

    // Enable normal mode for operation
    mcp2515_set_mode(&mcp2515, MCP_MODE_NORMAL);
}

// The CAN task function
void can_task(void *pvParams)
{
    uint8_t canCommand;                                // The general command to observe
    TickType_t lastWakeTime;                           // The last time we got a general command that wasn't idle
    const TickType_t tickInterval = pdMS_TO_TICKS(10); // 10 ms interval for checking commands

    // Set up SPI and CAN communication
    setup_spi();
    setup_can();

    // Create the CAN command queue
    canQueue = xQueueCreate(CAN_QUEUE_LENGTH, sizeof(uint8_t));
    if (canQueue == NULL)
    {
        // Handle error (e.g., print an error message)
        printf("Failed to create CAN command queue.\n");
        vTaskDelete(NULL); // Delete the task if the queue creation fails
    }

    // Initialize the last wake time for accurate timing
    lastWakeTime = xTaskGetTickCount();

    while (true)
    {
        // Check for new commands in the queue
        if (xQueueReceive(canQueue, &canCommand, 0) == pdPASS)
        {
            // Handle the received command
            if (canCommand == CAN_COMMAND_GET_VERSION)
            {
                // Retrieve the MCP2515 version
                uint8_t version = mcp2515_read_register(&mcp2515, MCP_CANSTAT);
                printf("MCP2515 Version: 0x%x\n", version);
            }
        }

        // Wait for the next cycle with a short interval to remain responsive
        vTaskDelayUntil(&lastWakeTime, tickInterval);
    }
}

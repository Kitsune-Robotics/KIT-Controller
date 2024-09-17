#include "can_controller.h"
#include "pico/stdlib.h"
#include "console.h"
#include "mcp2515.h"
#include "can.h"

// Constants for the CAN controller (We get these from the adafruit feather we're using)
const uint8_t CAN_QUEUE_LENGTH = 10;
const uint8_t CS_PIN = 19;          // Chip Select (CS) pin for the SPI
const uint8_t TX_PIN = 15;          // MOSI pin for SPI communication (TX)
const uint8_t RX_PIN = 8;           // MISO pin for SPI communication (RX)
const uint8_t SCK_PIN = 14;         // Clock pin for SPI communication (SCK)
const uint32_t SPI_CLOCK = 1000000; // 1 MHz SPI clock

// SPI instance for the MCP2515
spi_inst_t *spi_channel = spi0; // Use spi0 or spi1 based on your board

static QueueHandle_t canQueue;                                                // RTOS queue for CAN commands
static MCP2515 can0(spi_channel, CS_PIN, TX_PIN, RX_PIN, SCK_PIN, SPI_CLOCK); // Proper MCP2515 instance

struct can_frame rx;

// Function to request status info from the MCP2515
void request_can_status(Command_t *cmd)
{
    if (canQueue == NULL)
    {
        return;
    }

    uint8_t statusCommand = CAN_COMMAND_GET_STATUS;
    xQueueSend(canQueue, &statusCommand, 0);
}

// Function to send a CAN frame
void send_can_frame(Command_t *cmd)
{
    if (canQueue == NULL)
    {
        return;
    }

    uint8_t sendFrameCommand = CAN_COMMAND_SEND_FRAME;
    xQueueSend(canQueue, &sendFrameCommand, 0);
}

// Function to initialize CAN communication
void setup_can()
{
    // Initialize MCP2515 interface
    can0.reset();
    can0.setBitrate(CAN_500KBPS, MCP_16MHZ); // Use 500Kbps, typical for many setups
    can0.setNormalMode();
}

// The CAN task function
void can_task(void *pvParams)
{
    uint8_t canCommand;                                // The general command to observe
    TickType_t lastWakeTime;                           // The last time we got a general command that wasn't idle
    const TickType_t tickInterval = pdMS_TO_TICKS(10); // 10 ms interval for checking commands

    // Setup CAN communication
    setup_can();

    // Create the CAN command queue
    canQueue = xQueueCreate(CAN_QUEUE_LENGTH, sizeof(uint8_t));
    if (canQueue == NULL)
    {
        console_printf(BROADCAST, "Failed to create CAN command queue.\n");
        vTaskDelete(NULL); // Delete the task if the queue creation fails
    }

    // Initialize the last wake time for accurate timing
    lastWakeTime = xTaskGetTickCount();

    while (true)
    {
        // Check for new commands in the queue
        if (xQueueReceive(canQueue, &canCommand, 0) == pdPASS)
        {
            if (canCommand == CAN_COMMAND_GET_STATUS)
            {
                // Retrieve the MCP2515 status
                uint8_t status = can0.getStatus();
                console_printf(BROADCAST, "MCP2515 Status: 0x%x\n", status);
            }
            else if (canCommand == CAN_COMMAND_SEND_FRAME)
            {
                // Send a simple CAN frame
                struct can_frame txFrame;
                txFrame.can_id = 0x123; // Example CAN ID
                txFrame.can_dlc = 2;    // Data length (2 bytes)
                txFrame.data[0] = 0xDE; // First byte of data
                txFrame.data[1] = 0xAD; // Second byte of data

                if (can0.sendMessage(&txFrame) == MCP2515::ERROR_OK)
                {
                    console_printf(BROADCAST, "CAN frame sent successfully!\n");
                }
                else
                {
                    console_printf(BROADCAST, "Failed to send CAN frame.\n");
                }
            }
        }

        // Wait for the next cycle with a short interval to remain responsive
        vTaskDelayUntil(&lastWakeTime, tickInterval);
    }
}

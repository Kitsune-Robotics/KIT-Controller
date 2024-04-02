/**
 * @file main.cpp
 * @author Joe
 * @brief Source code for kw1fox-3
 */

// Pico Libs
#include <stdio.h>
#include <stdbool.h>
#include "pico/stdlib.h"

// FreeRTOS
#include "FreeRTOS.h"
#include "task.h"

// VFS
#include <VFS.h>

// Headers
#include "boardPins.h"

void setup()
{
    stdio_init_all();

    gpio_init(LED_BUILTIN);
    gpio_set_dir(LED_BUILTIN, GPIO_OUT);

    gpio_init(LED_GREEN);
    gpio_set_dir(LED_GREEN, GPIO_OUT);
}

void blink()
{
    gpio_put(LED_BUILTIN, 1);
    gpio_put(LED_GREEN, 1);
    sleep_ms(750);

    gpio_put(LED_BUILTIN, 0);
    gpio_put(LED_GREEN, 0);
    sleep_ms(1050);
}

int main()
{
    setup();

    printf("Hello World\n");

    while (true)
    {
        printf(".");
        blink();
    }

    return 0;
}
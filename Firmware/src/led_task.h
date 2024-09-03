#ifndef LED_TASK_H
#define LED_TASK_H

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

// Pindefs
#include "pindefs.h"

// Our console
#include "console.h"

// Function to initialize and start the LED task
void led_task(void *pvParams);

// Function to indicate activity (fast flash)
void busy(void);

#endif // LED_TASK_H

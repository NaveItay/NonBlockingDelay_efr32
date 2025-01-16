/*
 * NonBlockingDelay.h
 *
 * This library provides a mechanism for implementing non-blocking delays in embedded systems
 * using the SysTick timer. It allows you to perform delay operations without halting the processor,
 * enabling other tasks to run concurrently. This is particularly useful in embedded applications
 * where blocking delays (e.g., using `delay()` or `HAL_Delay()`) are undesirable because they
 * prevent the CPU from performing other tasks.
 *
 * **Library Overview:**
 *
 * - **SysTick Timer Initialization:**
 *   - The `NonBlockingDelay_initSysTick()` function initializes the SysTick timer to generate an
 *     interrupt every 1 millisecond. This sets up a global millisecond counter (`systick_millis`)
 *     that increments every millisecond.
 *
 * - **NonBlockingDelay Structure:**
 *   - The `NonBlockingDelay` struct holds the delay interval and the timestamp of the last reset.
 *     - `interval_ms`: The desired delay interval in milliseconds.
 *     - `previousMillis`: The millisecond count when the delay was last reset.
 *
 * - **Functions:**
 *   - `NonBlockingDelay_Init()`: Initializes a `NonBlockingDelay` instance with a specified interval.
 *   - `NonBlockingDelay_setInterval()`: Sets or updates the delay interval.
 *   - `NonBlockingDelay_reset()`: Resets the delay timer to start counting from the current time.
 *   - `NonBlockingDelay_check()`: Checks if the specified delay interval has elapsed.
 *
 * **Usage Example:**
 *
 * ```c
 * #include "NonBlockingDelay.h"
 *
 * int main(void)
 * {
 *     // Initialize the SysTick timer
 *     NonBlockingDelay_initSysTick();
 *
 *     // Create and initialize a NonBlockingDelay instance
 *     NonBlockingDelay myDelay;
 *     NonBlockingDelay_Init(&myDelay, 1000); // 1-second delay
 *
 *     while (1)
 *     {
 *         // Check if the delay interval has elapsed
 *         if (NonBlockingDelay_check(&myDelay))
 *         {
 *             // Perform the desired action
 *             // Example: Toggle an LED
 *             GPIO_PinOutToggle(gpioPortA, 0);
 *         }
 *
 *         // Other non-blocking tasks can be performed here
 *         // ...
 *     }
 * }
 * ```
 *
 * **Overflow Handling:**
 *
 * - The `systick_millis` variable is a 32-bit unsigned integer (`uint32_t`). It will overflow
 *   (wrap around to zero) after approximately 49.7 days (when it reaches `2^32` milliseconds).
 * - The subtraction operation in `NonBlockingDelay_check()`:
 *   ```c
 *   if ((currentMillis - delay->previousMillis) >= delay->interval_ms)
 *   ```
 *   correctly handles the overflow due to the properties of unsigned integer arithmetic in C.
 *   - If an overflow occurs, the result of the subtraction will still be the correct difference
 *     modulo `2^32`. This means the delay check remains accurate even when `systick_millis`
 *     wraps around.
 *
 * **Notes:**
 *
 * - Ensure that `NonBlockingDelay_initSysTick()` is called before using any other functions in
 *   this library to initialize the SysTick timer.
 * - The SysTick timer relies on the core clock. Be cautious when entering low-power modes that
 *   stop the core clock, as this will halt the SysTick timer and affect the delay timing.
 *
 *  Created on: Sep 20, 2023
 *      Author: itay
 */

#ifndef LIBRARIES_NONBLOCKINGDELAY_NONBLOCKINGDELAY_H_
#define LIBRARIES_NONBLOCKINGDELAY_NONBLOCKINGDELAY_H_

#include <stdint.h>
#include <stdbool.h>

// NonBlockingDelay structure definition
typedef struct
{
  uint32_t interval_ms;      // Delay interval in milliseconds
  uint32_t previousMillis;   // Last recorded millisecond count
} NonBlockingDelay;

// SysTick initialization function
void NonBlockingDelay_initSysTick(void);

// Non-blocking delay functions
void NonBlockingDelay_Init(NonBlockingDelay* delay, uint32_t interval_ms);
void NonBlockingDelay_setInterval(NonBlockingDelay* delay, uint32_t interval_ms);
void NonBlockingDelay_reset(NonBlockingDelay* delay);
bool NonBlockingDelay_check(NonBlockingDelay* delay);

#endif /* LIBRARIES_NONBLOCKINGDELAY_NONBLOCKINGDELAY_H_ */

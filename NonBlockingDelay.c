/*
 * NonBlockingDelay.c
 *
 * This file provides the implementation of the NonBlockingDelay functions, including
 * the SysTick timer initialization and the non-blocking delay mechanisms.
 *
 *  Created on: Sep 20, 2023
 *      Author: itay
 */

#include "NonBlockingDelay.h"
#include "em_device.h"
#include "em_cmu.h"
#include "em_core.h"

// Millisecond counter incremented by SysTick_Handler
volatile uint32_t systick_millis = 0;

/**
 * @brief SysTick interrupt handler.
 *
 * This function is called every time the SysTick timer reaches zero (every 1 ms).
 * It increments the millisecond counter.
 */
void SysTick_Handler(void)
{
  systick_millis++;
}

/**
 * @brief Initialize the SysTick timer to generate an interrupt every millisecond.
 *
 * This function configures the SysTick timer based on the core clock frequency
 * to generate periodic interrupts every 1 millisecond.
 */
void NonBlockingDelay_initSysTick(void)
{
  // Get the core clock frequency
  uint32_t coreFrequency = CMU_ClockFreqGet(cmuClock_CORE);

  // Calculate the number of ticks for 1 millisecond
  uint32_t ticks = coreFrequency / 1000;

  // Check if ticks is zero to prevent division by zero
  if (ticks == 0)
    {
      // Handle error: core clock frequency is zero
      while (1)
        ;
    }

  // Configure SysTick to generate an interrupt every millisecond
  if (SysTick_Config(ticks) != 0)
    {
      // Handle error: SysTick configuration failed
      while (1)
        ;
    }
}

/**
 * @brief Initialize a NonBlockingDelay instance with the specified interval.
 *
 * @param delay       Pointer to the NonBlockingDelay instance.
 * @param interval_ms Delay interval in milliseconds.
 */
void NonBlockingDelay_Init(NonBlockingDelay* delay, uint32_t interval_ms)
{
  NonBlockingDelay_setInterval(delay, interval_ms);
  NonBlockingDelay_reset(delay);
}

/**
 * @brief Set the delay interval for a NonBlockingDelay instance.
 *
 * @param delay       Pointer to the NonBlockingDelay instance.
 * @param interval_ms Delay interval in milliseconds.
 */
void NonBlockingDelay_setInterval(NonBlockingDelay* delay, uint32_t interval_ms)
{
  delay->interval_ms = interval_ms;
}

/**
 * @brief Reset the delay timer for a NonBlockingDelay instance.
 *
 * This function resets the previous millisecond count to the current time,
 * effectively restarting the delay period.
 *
 * @param delay Pointer to the NonBlockingDelay instance.
 */
void NonBlockingDelay_reset(NonBlockingDelay* delay)
{
  // Atomically read the current millisecond count
  CORE_ATOMIC_SECTION(
      delay->previousMillis = systick_millis;
  )
}

/**
 * @brief Check if the delay interval has elapsed.
 *
 * This function checks whether the specified delay interval has passed since
 * the last reset. If the interval has passed, it updates the previous millisecond
 * count to the current time.
 *
 * @param delay Pointer to the NonBlockingDelay instance.
 * @return true if the delay interval has elapsed, false otherwise.
 */
bool NonBlockingDelay_check(NonBlockingDelay* delay)
{
  uint32_t currentMillis;

  // Atomically read the current millisecond count
  CORE_ATOMIC_SECTION(
      currentMillis = systick_millis;
  )

  if ((currentMillis - delay->previousMillis) >= delay->interval_ms)
    {
      // Interval has passed, update previousMillis
      delay->previousMillis = currentMillis;
      return true;
    }
  else
    {
      return false;
    }
}

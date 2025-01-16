# NonBlockingDelay Library

The `NonBlockingDelay` library provides a mechanism to implement non-blocking delays in embedded systems using the SysTick timer. Unlike traditional blocking delay methods (`delay()` or `HAL_Delay()`), this library allows the CPU to continue executing other tasks while the delay is active, making it ideal for real-time applications.

---

## Features

- **Non-blocking Delays**: Perform delays without halting CPU execution.
- **SysTick Timer Integration**: Uses the SysTick timer to generate 1 ms interrupts for precise timekeeping.
- **Customizable Delay Intervals**: Supports delays for any duration in milliseconds.
- **Overflow-Safe**: Handles the 32-bit millisecond counter overflow correctly for long runtime operations.
- **Lightweight**: Minimal overhead and easy integration into existing projects.

---

## Getting Started

### Prerequisites

- **Hardware**: EFR32 microcontroller (or any MCU with SysTick support).
- **Software**: Embedded C development environment (e.g., Silicon Labs Simplicity Studio).

---

## Installation

1. Clone or download the repository:
   ```bash
   git clone https://github.com/NaveItay/NonBlockingDelay_efr32.git
2. Include the `NonBlockingDelay.h` and `NonBlockingDelay.c` files in your project directory.
3. Add the necessary include paths in your compiler settings to ensure the library files are recognized.
4. Make sure your project includes the required device-specific headers for SysTick and clock management.

---

## API Reference

### Initialization

- **`void NonBlockingDelay_initSysTick(void)`**  
  Initializes the SysTick timer to generate an interrupt every millisecond. This function must be called before using any delay-related features.

### Delay Management

- **`void NonBlockingDelay_Init(NonBlockingDelay* delay, uint32_t interval_ms)`**  
  Initializes a `NonBlockingDelay` instance with a specified delay interval in milliseconds.

- **`void NonBlockingDelay_setInterval(NonBlockingDelay* delay, uint32_t interval_ms)`**  
  Updates or sets the delay interval for a `NonBlockingDelay` instance.

- **`void NonBlockingDelay_reset(NonBlockingDelay* delay)`**  
  Resets the delay timer, starting the interval count from the current millisecond time.

- **`bool NonBlockingDelay_check(NonBlockingDelay* delay)`**  
  Checks if the specified delay interval has elapsed. Returns `true` if the interval has passed, `false` otherwise.

---

## Usage Example

```c
#include "NonBlockingDelay.h"

int main(void)
{
    // Initialize the SysTick timer
    NonBlockingDelay_initSysTick();

    // Create and initialize a NonBlockingDelay instance
    NonBlockingDelay myDelay;
    NonBlockingDelay_Init(&myDelay, 1000); // 1-second delay

    while (1)
    {
        // Check if the delay interval has elapsed
        if (NonBlockingDelay_check(&myDelay))
        {
            // Perform an action, e.g., toggle an LED
            GPIO_PinOutToggle(gpioPortA, 0);
        }

        // Other tasks can run here without being blocked
    }
}

```

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

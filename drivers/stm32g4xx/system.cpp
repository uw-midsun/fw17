#include "drivers/stm32g4xx/system.h"

#include "stm32g4xx_hal.h"

extern "C" {

void SysTick_Handler(void) { HAL_IncTick(); }

// Required syscalls.
void _init(void) {}
void _fini(void) {}
}

namespace midsun::drivers::system {

void Init() { HAL_Init(); }

}  // namespace midsun::drivers::system

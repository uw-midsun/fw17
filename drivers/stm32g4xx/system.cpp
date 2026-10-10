#include "drivers/stm32g4xx/system.h"

#include "stm32g4xx_hal.h"

namespace midsun::drivers::system {

void Init() { HAL_Init(); }

}  // namespace midsun::drivers::system

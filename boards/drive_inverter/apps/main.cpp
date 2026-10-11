#include <array>

#include "boards/drive_inverter/drive_inverter.h"
#include "stm32g4xx_hal/stm32g4xx_hal.h"

int main() {
  auto board = midsun::boards::drive_inverter::Init();

  while(true) {
    board.blinky_pin.Toggle();
    //TODO: Remove hal usage.
    HAL_Delay(200U);
  }
}

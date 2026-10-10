#include <array>

#include "drivers/stm32g4xx/gpio.h"
#include "drivers/stm32g4xx/system.h"
#include "stm32g4xx_hal/stm32g4xx_hal.h"

int main() {
  midsun::drivers::stm32g4xx::system::Init();

  using BlinkyPin = midsun::drivers::stm32g4xx::gpio::OutputPin<
      midsun::drivers::stm32g4xx::gpio::Port::C, midsun::drivers::stm32g4xx::gpio::Pin::Pin13,
      midsun::drivers::stm32g4xx::gpio::Mode::PushPullOutput, midsun::drivers::stm32g4xx::gpio::Pull::Up,
      midsun::drivers::stm32g4xx::gpio::Speed::VeryHigh>;

  auto blinky_pin = BlinkyPin::Init();

  while(true) {
    blinky_pin.Toggle();
    HAL_Delay(200U);
  }
}

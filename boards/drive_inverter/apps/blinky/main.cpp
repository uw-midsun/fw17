#include <array>

#include "drivers/stm32g4xx/gpio.h"
#include "drivers/stm32g4xx/system.h"
#include "stm32g4xx_hal/stm32g4xx_hal.h"

namespace {

namespace driver_gpio = midsun::drivers::stm32g4xx::gpio;

}

int main() {
  midsun::drivers::stm32g4xx::system::Init();

  using BlinkyPin =
      driver_gpio::OutputPin<driver_gpio::Port::C, driver_gpio::Pin::Pin13, driver_gpio::Mode::PushPullOutput,
                             driver_gpio::Pull::Up, driver_gpio::Speed::VeryHigh>;

  auto blinky_pin = BlinkyPin::Init();

  while(true) {
    blinky_pin.Toggle();
    HAL_Delay(200U);
  }
}

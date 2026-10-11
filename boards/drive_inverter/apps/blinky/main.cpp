#include <array>

#include "drivers/stm32g4xx/gpio.h"
#include "drivers/stm32g4xx/system.h"
#include "stm32g4xx_hal/stm32g4xx_hal.h"

namespace {

namespace driver_gpio = midsun::drivers::stm32g4xx::gpio;

struct BlinkyPin {
  static constexpr auto kPort = driver_gpio::Port::C;
  static constexpr auto kPin = driver_gpio::Pin::Pin13;
  static constexpr auto kMode = driver_gpio::Mode::PushPullOutput;
  static constexpr auto kPull = driver_gpio::Pull::Up;
  static constexpr auto kSpeed = driver_gpio::Speed::High;
};

}  // namespace

int main() {
  midsun::drivers::stm32g4xx::system::Init();
  auto blinky_pin = driver_gpio::OutputPin<BlinkyPin>::Init();

  while(true) {
    blinky_pin.Toggle();
    HAL_Delay(200U);
  }
}

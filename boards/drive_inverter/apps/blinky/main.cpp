#include <array>

#include "drivers/stm32g4xx/gpio.h"
#include "drivers/stm32g4xx/system.h"
#include "stm32g4xx_hal/stm32g4xx_hal.h"

namespace {

namespace stm = midsun::drivers::stm32g4xx;

struct BlinkyPin {
  static constexpr auto kPort = stm::gpio::Port::C;
  static constexpr auto kPin = stm::gpio::Pin::Pin13;
  static constexpr auto kMode = stm::gpio::Mode::PushPullOutput;
  static constexpr auto kPull = stm::gpio::Pull::Up;
  static constexpr auto kSpeed = stm::gpio::Speed::High;
};

}  // namespace

int main() {
  midsun::drivers::stm32g4xx::system::Init();
  auto blinky_pin = stm::gpio::OutputPin<BlinkyPin>::Init();

  while(true) {
    blinky_pin.Toggle();
    HAL_Delay(200U);
  }
}

#include "boards/drive_inverter/drive_inverter.h"

#include "drivers/stm32g4xx/gpio.h"
#include "drivers/stm32g4xx/system.h"

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

namespace midsun::boards::drive_inverter {

DriveInverter Init() noexcept {
  stm::system::Init();

  static auto blinky_pin = stm::gpio::OutputPin<BlinkyPin>::Init();

  return DriveInverter{.blinky_pin = blinky_pin};
}
}  // namespace midsun::boards::drive_inverter

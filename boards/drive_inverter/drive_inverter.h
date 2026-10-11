#pragma once

#include "drivers/gpio.h"

namespace midsun::boards::drive_inverter {

struct DriveInverter {
  drivers::gpio::OutputPin& blinky_pin;
};

DriveInverter Init() noexcept;

}  // namespace midsun::boards::drive_inverter

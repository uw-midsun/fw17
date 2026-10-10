#pragma once

#include <cstdint>
#include <optional>

#include "stm32g4xx_hal/stm32g4xx_hal.h"

namespace midsun::drivers::stm32g4xx::gpio {

enum class Pin {
  Pin0 = GPIO_PIN_0,
  Pin1 = GPIO_PIN_1,
  Pin2 = GPIO_PIN_2,
  Pin3 = GPIO_PIN_3,
  Pin4 = GPIO_PIN_4,
  Pin5 = GPIO_PIN_5,
  Pin6 = GPIO_PIN_6,
  Pin7 = GPIO_PIN_7,
  Pin8 = GPIO_PIN_8,
  Pin9 = GPIO_PIN_9,
  Pin10 = GPIO_PIN_10,
  Pin11 = GPIO_PIN_11,
  Pin12 = GPIO_PIN_12,
  Pin13 = GPIO_PIN_13,
  Pin14 = GPIO_PIN_14,
  Pin15 = GPIO_PIN_15,
  All = GPIO_PIN_0,
};

constexpr bool IsValidPin(Pin pin) { return IS_GPIO_PIN(pin); }

enum class Port : std::uint8_t { A = 0U, B, C, D, E, F, G };

constexpr bool IsValidPort(Port port) {
  switch(port) {
    case Port::A:
    case Port::B:
    case Port::C:
    case Port::D:
    case Port::E:
    case Port::F:
    case Port::G:
      return true;
  }
  return false;
}

namespace private_ {

using GpioPort = GPIO_TypeDef*;

constexpr std::optional<GpioPort> GetInstance(const Port& port) {
  switch(port) {
    case Port::A:
      return GPIOA;
    case Port::B:
      return GPIOB;
    case Port::C:
      return GPIOC;
    case Port::D:
      return GPIOD;
    case Port::E:
      return GPIOE;
    case Port::F:
      return GPIOF;
    case Port::G:
      return GPIOG;
    default:
      return std::nullopt;
  }
}

}  // namespace private_

}  // namespace midsun::drivers::stm32g4xx::gpio

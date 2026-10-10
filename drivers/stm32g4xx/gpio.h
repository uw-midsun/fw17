#pragma once

#include <assert.h>

#include <cstdint>
#include <optional>

#include "drivers/gpio.h"
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

enum class Mode {
  Input = GPIO_MODE_INPUT,
  PushPullOutput = GPIO_MODE_OUTPUT_PP,
  OpenDrainOutput = GPIO_MODE_OUTPUT_OD,
  AlternateFunctionPushPull = GPIO_MODE_AF_PP,
  AlternateFunctionOpenDrain = GPIO_MODE_AF_OD,
  InterruptRising = GPIO_MODE_IT_RISING,
  InterruptFalling = GPIO_MODE_IT_FALLING,
  InterruptRisingFalling = GPIO_MODE_IT_RISING_FALLING,
  EventRising = GPIO_MODE_EVT_RISING,
  EventFalling = GPIO_MODE_EVT_FALLING,
  EventRisingFalling = GPIO_MODE_EVT_RISING_FALLING,
  Analog = GPIO_MODE_ANALOG,
};

constexpr bool IsValidMode(const Mode mode) { return IS_GPIO_MODE(static_cast<uint32_t>(mode)); }

constexpr bool IsOutputMode(const Mode mode) {
  return (mode == Mode::PushPullOutput || mode == Mode::OpenDrainOutput) && IsValidMode(mode);
}

enum class Pull {
  Up = GPIO_PULLUP,
  Down = GPIO_PULLDOWN,
  None = GPIO_NOPULL,
};

constexpr bool IsValidPull(const Pull pull) { return IS_GPIO_PULL(static_cast<uint32_t>(pull)); }

enum class Speed {
  Low = GPIO_SPEED_FREQ_LOW,
  Medium = GPIO_SPEED_FREQ_MEDIUM,
  High = GPIO_SPEED_FREQ_HIGH,
  VeryHigh = GPIO_SPEED_FREQ_VERY_HIGH,
};

constexpr bool IsValidSpeed(const Speed speed) { return IS_GPIO_SPEED(static_cast<uint32_t>(speed)); }

enum class Port : std::uint8_t { A = 0U, B, C, D, E, F, G };

using GpioInstance = GPIO_TypeDef*;

constexpr std::optional<GpioInstance> GetInstance(const Port& port) {
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

template <Port kPort, Pin kPin, Mode kMode, Pull kPull, Speed kSpeed>
class OutputPin final : public midsun::drivers::gpio::OutputPin {
  static constexpr auto kMaybeInstance = GetInstance(kPort);
  static constexpr auto kPinCasted = static_cast<std::uint16_t>(kPin);

  static_assert(IsValidPin(kPin));
  static_assert(IsValidMode(kMode));
  static_assert(IsValidPull(kPull));
  static_assert(IsValidSpeed(kSpeed));
  static_assert(kMaybeInstance.has_value());

  void Init() noexcept override {
    auto instance = OutputPin{kMaybeInstance.value()};

    const auto init = GPIO_InitTypeDef{
        .Pin = kPin,
        .Mode = kMode,
        .Speed = kSpeed,
        .Alternate = 0U,
    };

    HAL_GPIO_Init(instance_, &init);
  }

  midsun::drivers::gpio::Direction GetOutput() const noexcept override {
    const auto state = HAL_GPIO_ReadPin(instance_, kPinCasted);
    if(state == GPIO_PIN_RESET) {
      return midsun::drivers::gpio::Direction::Low;
    }
    return midsun::drivers::gpio::Direction::High;
  }

  void SetOutput(const midsun::drivers::gpio::Direction& direction) noexcept override {
    auto hal_direction = GPIO_PIN_SET;
    if(direction == midsun::drivers::gpio::Direction::Low) {
      hal_direction = GPIO_PIN_RESET;
    }

    HAL_GPIO_WritePin(instance_, kPinCasted, hal_direction);
  }

  midsun::drivers::gpio::Direction Toggle() noexcept override {
    HAL_GPIO_TogglePin(instance_, kPinCasted);

    return GetOutput();
  }

 private:
  constexpr OutputPin(const GpioInstance& instance) : instance_{instance} {}

  GpioInstance instance_;
};

}  // namespace midsun::drivers::stm32g4xx::gpio

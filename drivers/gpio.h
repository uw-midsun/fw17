#pragma once

#include <tl/expected.h>

#include <cstdint>

namespace midsun::drivers::gpio {

enum class Error : std::uint8_t {
  InitFailed,
};

enum class Direction : std::uint8_t {
  Low = 0U,
  High = 1U,
};

class OutputPin {
  virtual tl::expected<void, Error> Init() noxecept = 0U;

  virtual Direction GetOutput() noexcept = 0U;

  virtual SetOutput(const Direction& direction) noexcept = 0U;

  // The intended interface of toggle is for it to return the new `Direction`.
  virtual Direction Toggle() noexcept = 0U;
};

}  // namespace midsun::drivers::gpio

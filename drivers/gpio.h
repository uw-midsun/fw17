#pragma once

#include <cstdint>

namespace midsun::drivers::gpio {

enum class Error : std::uint8_t {
  InitFailed = 0U,
};

enum class Direction : std::uint8_t {
  Low = 0U,
  High,
};

class OutputPin {
 public:
  virtual Direction GetOutput() const noexcept = 0;

  virtual void SetOutput(const Direction& direction) noexcept = 0;

  // The intended interface of toggle is for it to return the new `Direction`.
  virtual Direction Toggle() noexcept = 0;
};

}  // namespace midsun::drivers::gpio

#pragma once

#include <cstdint>

namespace midsun::drivers::gpio {

enum class Level : std::uint8_t {
  Low = 0U,
  High,
};

class OutputPin {
 public:
  virtual void SetOutput(const Level& level) noexcept = 0;
  virtual void Toggle() noexcept = 0;
};

}  // namespace midsun::drivers::gpio

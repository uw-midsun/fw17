#pragma once

#include <cstdint>
#include <span>

namespace midsun::drivers::crc {

template <typename ReturnType>
class Crc {
 public:
  virtual ReturnType Calculate(std::span<const uint8_t> data) noexcept = 0;
};

}  // namespace midsun::drivers::crc

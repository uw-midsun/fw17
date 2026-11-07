#pragma once

#include <cstdint>

namespace midsun::drivers::crc {

template <typename ReturnType>
class Crc {
 public:
  virtual ReturnType Calculate(const uint8_t *pBuffer, uint32_t BufferLength) noexcept = 0;
};

}  // namespace midsun::drivers::crc

#pragma once

#include <cstddef>
#include <span>

namespace midsun::drivers {

template <typename ReturnType>
class Crc {
 public:
  virtual ReturnType Calculate(const std::span<const std::byte> data) noexcept = 0;
};

}  // namespace midsun::drivers

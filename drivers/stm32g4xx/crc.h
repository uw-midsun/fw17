#pragma once

#include <bit>
#include <concepts>
#include <type_traits>

#include "drivers/crc.h"
#include "stm32g4xx_hal/stm32g4xx_hal.h"

namespace midsun::drivers::stm32g4xx::crc {

enum class Length {
  Length32 = CRC_POLYLENGTH_32B,
  Length16 = CRC_POLYLENGTH_16B,
  Length8 = CRC_POLYLENGTH_8B,
};

constexpr bool IsValidLength(const Length& length) {
  switch(length) {
    case Length::Length32:
    case Length::Length16:
    case Length::Length8:
      return true;
  }
  return false;
}

enum class InputInversionMode {
  None = CRC_INPUTDATA_INVERSION_NONE,
  Byte = CRC_INPUTDATA_INVERSION_BYTE,
  HalfWord = CRC_INPUTDATA_INVERSION_HALFWORD,
  Word = CRC_INPUTDATA_INVERSION_WORD,
};

constexpr bool IsValidInputInversionMode(const InputInversionMode& input_inversion_mode) {
  switch(input_inversion_mode) {
    case InputInversionMode::None:
    case InputInversionMode::Byte:
    case InputInversionMode::HalfWord:
    case InputInversionMode::Word:
      return true;
  }
  return false;
}

enum class OutputInversionMode {
  Enable = CRC_OUTPUTDATA_INVERSION_ENABLE,
  Disable = CRC_OUTPUTDATA_INVERSION_DISABLE,
};

constexpr bool IsValidOutputInversionMode(const OutputInversionMode& output_inversion_mode) {
  switch(output_inversion_mode) {
    case OutputInversionMode::Enable:
    case OutputInversionMode::Disable:
      return true;
  }
  return false;
}

// Config standards based on AUTOSAR CRC Specs
struct Crc32Config {  // IEEE 803.2 CRC32 Ethernet Standard
  static constexpr auto kPoly = uint32_t{0x04C11DB7};
  static constexpr auto kLength = Length::Length32;
  static constexpr auto kInit = uint32_t{0xFFFFFFFF};
  static constexpr auto kInputInversionMode = InputInversionMode::Byte;
  static constexpr auto kOutputInversionMode = OutputInversionMode::Enable;
  static constexpr auto kXor = uint32_t{0xFFFFFFFF};
  static constexpr auto kCheck = uint32_t{0xCBF43926};
  static constexpr auto kResidue = uint32_t{0xDEBB20E3};
};

struct Crc16Config {  // CCITT-FALSE CRC16 Standard
  static constexpr auto kPoly = uint32_t{0x1021};
  static constexpr auto kLength = Length::Length16;
  static constexpr auto kInit = uint32_t{0xFFFF};
  static constexpr auto kInputInversionMode = InputInversionMode::None;
  static constexpr auto kOutputInversionMode = OutputInversionMode::Disable;
  static constexpr auto kXor = uint32_t{0x0000};
  static constexpr auto kCheck = uint32_t{0x29B1};
  static constexpr auto kResidue = uint32_t{0x0000};
};

struct Crc8Config {  // SAE-J1850 CRC8 Standard
  static constexpr auto kPoly = uint32_t{0x1D};
  static constexpr auto kLength = Length::Length8;
  static constexpr auto kInit = uint32_t{0xFF};
  static constexpr auto kInputInversionMode = InputInversionMode::None;
  static constexpr auto kOutputInversionMode = OutputInversionMode::Disable;
  static constexpr auto kXor = uint32_t{0xFF};
  static constexpr auto kCheck = uint32_t{0x4B};
  static constexpr auto kResidue = uint32_t{0xC4};
};

template <typename T>
concept UnsignedIntegral = std::is_integral_v<T> && std::is_unsigned_v<T>;

template <Length L>
using ReturnTypeFor =
    std::conditional_t<L == Length::Length8, uint8_t, std::conditional_t<L == Length::Length16, uint16_t, uint32_t>>;

template <typename Config>
class Crc final : public midsun::drivers::crc::Crc<ReturnTypeFor<Config::kLength>> {
  static_assert(UnsignedIntegral<decltype(Config::kPoly)>);
  static_assert(UnsignedIntegral<decltype(Config::kInit)>);
  static_assert(IsValidLength(Config::kLength));
  static_assert(IsValidInputInversionMode(Config::kInputInversionMode));
  static_assert(IsValidOutputInversionMode(Config::kOutputInversionMode));

  using ReturnType = ReturnTypeFor<Config::kLength>;
  static_assert(UnsignedIntegral<ReturnType>);

 public:
  static Crc Init() noexcept {
    __HAL_RCC_CRC_CLK_ENABLE();

    constexpr auto kInit = CRC_InitTypeDef{
        .DefaultPolynomialUse = DEFAULT_POLYNOMIAL_DISABLE,
        .DefaultInitValueUse = DEFAULT_INIT_VALUE_DISABLE,
        .GeneratingPolynomial = static_cast<uint32_t>(Config::kPoly),
        .CRCLength = static_cast<uint32_t>(Config::kLength),
        .InitValue = static_cast<uint32_t>(Config::kInit),
        .InputDataInversionMode = static_cast<uint32_t>(Config::kInputInversionMode),
        .OutputDataInversionMode = static_cast<uint32_t>(Config::kOutputInversionMode),
    };

    auto handle = CRC_HandleTypeDef{
        .Instance = CRC,
        .Init = kInit,
        .Lock = HAL_UNLOCKED,
        .State = HAL_CRC_STATE_RESET,
        .InputDataFormat = CRC_INPUTDATA_FORMAT_BYTES,
    };
    auto crc = Crc{handle};

    HAL_CRC_Init(&crc.handle_);
    return crc;
  }

  ReturnType Calculate(std::span<const uint8_t> data) noexcept override {
    // Cast uint8_t data to uint32_t data for HAL function type matching,
    // CRC_INPUTDATA_FORMAT_BYTES above ensures data gets casted back to uint8_t before being used
    const auto span_data = std::bit_cast<const uint32_t*>(data.data());
    const auto span_size = static_cast<uint32_t>(data.size());
    auto result = HAL_CRC_Calculate(&this->handle_, span_data, span_size);

    // XOR result to match AUTOSAR CRC spec
    result ^= Config::kXor;

    return static_cast<ReturnType>(result);
  }

 private:
  constexpr Crc(const CRC_HandleTypeDef& handle) : handle_(handle) {}

  CRC_HandleTypeDef handle_;
};

}  // namespace midsun::drivers::stm32g4xx::crc
#pragma once

#include <concepts>
#include <cstdint>
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
  switch (length) {
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

constexpr bool IsValidInputInversionMode(const InputInversionMode& inputInversionMode) {
  switch (inputInversionMode) {
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

constexpr bool IsValidOutputInversionMode(const OutputInversionMode& outputInversionMode) {
  switch (outputInversionMode) {
    case OutputInversionMode::Enable:
    case OutputInversionMode::Disable:
      return true;
  }
  return false;
}


// Config standards based on AUTOSAR CRC Specs
struct Crc32Config {  // IEEE 803.2 CRC32 Ethernet Standard
  static constexpr uint32_t kPoly = 0x04C11DB7;
  static constexpr Length kLength = Length::Length32;
  static constexpr uint32_t kInit = 0xFFFFFFFF;
  static constexpr InputInversionMode kInputInversionMode = InputInversionMode::Byte;
  static constexpr OutputInversionMode kOutputInversionMode = OutputInversionMode::Enable;
  static constexpr uint32_t kXor = 0xFFFFFFFF;
  static constexpr uint32_t kCheck = 0xCBF43926;
  static constexpr uint32_t kResidue = 0xDEBB20E3;
};

struct Crc16Config {  // CCITT-FALSE CRC16 Standard
  static constexpr uint32_t kPoly = 0x1021;
  static constexpr Length kLength = Length::Length16;
  static constexpr uint32_t kInit = 0xFFFF;
  static constexpr InputInversionMode kInputInversionMode = InputInversionMode::None;
  static constexpr OutputInversionMode kOutputInversionMode = OutputInversionMode::Disable;
  static constexpr uint32_t kXor = 0x0000;
  static constexpr uint32_t kCheck = 0x29B1;
  static constexpr uint32_t kResidue = 0x0000;
};

struct Crc8Config {   // SAE-J1850 CRC8 Standard
  static constexpr uint32_t kPoly = 0x1D;
  static constexpr Length kLength = Length::Length8;
  static constexpr uint32_t kInit = 0xFF;
  static constexpr InputInversionMode kInputInversionMode = InputInversionMode::None;
  static constexpr OutputInversionMode kOutputInversionMode = OutputInversionMode::Disable;
  static constexpr uint32_t kXor = 0xFF;
  static constexpr uint32_t kCheck = 0x4B;
  static constexpr uint32_t kResidue = 0xC4;
};

template <typename T>
concept UnsignedIntegral = std::is_integral_v<T> && std::is_unsigned_v<T>;

template <Length L>
using ReturnTypeFor = std::conditional_t<L == Length::Length8, uint8_t,
                        std::conditional_t<L == Length::Length16, uint16_t, uint32_t>>;

template <typename Config>
class Crc final : public midsun::drivers::crc::Crc<ReturnTypeFor<Config::kLength>>  {
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
    
    auto crc = Crc{};
    crc.handle_.Instance = CRC;
    crc.handle_.Init = kInit;
    crc.handle_.InputDataFormat = CRC_INPUTDATA_FORMAT_BYTES;

    HAL_CRC_Init(&crc.handle_);

    return crc;
  }

  ReturnType Calculate(const uint8_t *pBuffer, uint32_t BufferLength) noexcept override {
    uint32_t result = HAL_CRC_Calculate(&this->handle_, reinterpret_cast<const uint32_t *>(pBuffer), BufferLength);

    result ^= Config::kXor;

    return static_cast<ReturnType>(result);
  }

 private:
  constexpr Crc() = default;

  CRC_HandleTypeDef handle_;
};

}
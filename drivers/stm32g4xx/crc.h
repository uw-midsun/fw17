#pragma once

#include <cstdint>

#include "stm32g4xx_hal/stm32g4xx_hal.h"

namespace midsun::drivers::stm32g4xx::crc {

// Config standards based on AUTOSAR CRC Specs
struct Crc32Config {  // IEEE 803.2 CRC32 Ethernet Standard
  using ReturnType = uint32_t;
  static constexpr uint32_t kPoly = 0x04C11DB7;
  static constexpr uint32_t kLength = CRC_POLYLENGTH_32B;
  static constexpr uint32_t kInit = 0xFFFFFFFF;
  static constexpr uint32_t kInputInversionMode = CRC_INPUTDATA_INVERSION_BYTE;
  static constexpr uint32_t kOutputInversionMode = CRC_OUTPUTDATA_INVERSION_ENABLE;
  static constexpr uint32_t kXor = 0xFFFFFFFF;
  static constexpr uint32_t kCheck = 0xCBF43926;
  static constexpr uint32_t kResidue = 0xDEBB20E3;
};

struct Crc16Config {  // CCITT-FALSE CRC16 Standard
  using ReturnType = uint16_t;
  static constexpr uint32_t kPoly = 0x1021;
  static constexpr uint32_t kLength = CRC_POLYLENGTH_16B;
  static constexpr uint32_t kInit = 0xFFFF;
  static constexpr uint32_t kInputInversionMode = CRC_INPUTDATA_INVERSION_NONE;
  static constexpr uint32_t kOutputInversionMode = CRC_OUTPUTDATA_INVERSION_DISABLE;
  static constexpr uint32_t kXor = 0x0000;
  static constexpr uint32_t kCheck = 0x29B1;
  static constexpr uint32_t kResidue = 0x0000;
};

struct Crc8Config {   // SAE-J1850 CRC8 Standard
  using ReturnType = uint8_t;
  static constexpr uint32_t kPoly = 0x1D;
  static constexpr uint32_t kLength = CRC_POLYLENGTH_8B;
  static constexpr uint32_t kInit = 0xFF;
  static constexpr uint32_t kInputInversionMode = CRC_INPUTDATA_INVERSION_NONE;
  static constexpr uint32_t kOutputInversionMode = CRC_OUTPUTDATA_INVERSION_DISABLE;
  static constexpr uint32_t kXor = 0xFF;
  static constexpr uint32_t kCheck = 0x4B;
  static constexpr uint32_t kResidue = 0xC4;
};

template <typename Config>
class Crc final {
  static_assert(IS_CRC_POL_LENGTH(Config::kLength));

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

  typename Config::ReturnType Calculate(const uint8_t *pBuffer, uint32_t BufferLength) noexcept {
    uint32_t result = HAL_CRC_Calculate(&this->handle_, reinterpret_cast<const uint32_t *>(pBuffer), BufferLength);

    result ^= Config::kXor;

    return static_cast<typename Config::ReturnType>(result);
  }

 private:
  constexpr Crc() = default;

  CRC_HandleTypeDef handle_;
};

}
#include "drivers/stm32g4xx/crc.h"

#include <array>
#include <cassert>

#include "drivers/stm32g4xx/system.h"
#include "stm32g4xx_hal/stm32g4xx_hal.h"

namespace stm = midsun::drivers::stm32g4xx;

static volatile bool checkOk;
static volatile bool residueOk;

int main() {
  stm::system::Init();

  auto crc = stm::crc::Crc<stm::crc::Crc32Config>::Init();
  const std::array<uint8_t, 9> checkData{'1', '2', '3', '4', '5', '6', '7', '8', '9'};
  const std::array<uint8_t, 13> residueData{'1', '2', '3', '4', '5', '6', '7', '8', '9', 0xCB, 0xF4, 0x39, 0x26};

  const uint32_t checkResult = crc.Calculate(checkData);
  const uint32_t residueResult = crc.Calculate(residueData);

  checkOk = (checkResult == stm::crc::Crc32Config::kCheck);
  residueOk = (residueResult == stm::crc::Crc32Config::kResidue);
  while(true) {
  }
}

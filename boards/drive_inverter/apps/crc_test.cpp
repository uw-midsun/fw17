#include <array>
#include <cassert>

#include "stm32g4xx_hal/stm32g4xx_hal.h"
#include "drivers/stm32g4xx/crc.h"
#include "drivers/stm32g4xx/system.h"

namespace stm = midsun::drivers::stm32g4xx;

static volatile bool checkOk;
static volatile bool residueOk;

int main() {
  stm::system::Init();

  auto crc = stm::crc::Crc<stm::crc::Crc32Config>::Init();
  const uint8_t *checkData = reinterpret_cast<const uint8_t *>("123456789");
  const uint8_t *residueData = reinterpret_cast<const uint8_t *>("123456789Ëô9&");
  const uint32_t checkResult = crc.Calculate(checkData, sizeof(uint8_t) * 9);
  const uint32_t residueResult = crc.Calculate(residueData, sizeof(uint8_t) * 13);
  
  checkOk = (checkResult == stm::crc::Crc32Config::kCheck);
  residueOk = (residueResult == stm::crc::Crc32Config::kResidue);
  while (true) {}
}

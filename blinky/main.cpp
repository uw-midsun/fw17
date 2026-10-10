#include <array>

#include "drivers/stm32g4xx/system.h"
#include "stm32g4xx_hal/stm32g4xx_hal.h"

extern "C" void SysTick_Handler(void) { HAL_IncTick(); }

int main() {
  midsun::drivers::system::Init();

  __HAL_RCC_GPIOC_CLK_ENABLE();

  auto* gpio_port = GPIOC;
  constexpr auto kPin = GPIO_PIN_13;

  GPIO_InitTypeDef init = {
      .Pin = kPin,
      .Mode = GPIO_MODE_OUTPUT_PP,
      .Pull = GPIO_PULLUP,
      .Speed = GPIO_SPEED_FREQ_HIGH,
      .Alternate = 0U,
  };

  HAL_GPIO_Init(gpio_port, &init);

  while (true) {
    HAL_GPIO_TogglePin(gpio_port, kPin);
    HAL_Delay(200U);
  }
}

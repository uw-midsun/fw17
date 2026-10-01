#include <array>
#include "stm32g4xx_hal/stm32g4xx_hal.h"

int main() {
  HAL_Init();
  HAL_InitTick(32U);

  auto* gpio_port = GPIOA;
  constexpr auto kPin = GPIO_PIN_0;

  GPIO_InitTypeDef init = {
    .Pin = kPin,
    .Mode = GPIO_MODE_OUTPUT_PP,
    .Pull = GPIO_PULLUP,
    .Speed = GPIO_SPEED_FREQ_HIGH,
    .Alternate = 0U,
  };

  HAL_GPIO_Init(gpio_port, &init);

  while(true) {
    HAL_GPIO_TogglePin(gpio_port, kPin);
    HAL_Delay(200U);
  }
}

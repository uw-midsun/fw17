#include <array>
#include "stm32g4xx_hal/stm32g4xx_hal.h"

int main() {
  GPIO_InitTypeDef init = {
    .Pin = GPIO_PIN_0,
    .Mode = GPIO_MODE_OUTPUT_PP,
    .Pull = GPIO_PULLUP,
    .Speed = GPIO_SPEED_FREQ_HIGH,
    .Alternate = 0U,
  };

  HAL_GPIO_Init(GPIOA, &init);

  while(true) {
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);
  }
}

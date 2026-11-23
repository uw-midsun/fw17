#include "FreeRTOS/FreeRTOS.h"
#include "FreeRTOS/task.h"

extern "C" void vApplicationIdleHook() {}

extern "C" void vApplicationStackOverflowHook(TaskHandle_t, char*) {}

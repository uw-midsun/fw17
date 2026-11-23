#pragma once

#include <chrono>
#include <concepts>
#include <functional>
#include <string_view>

#include "FreeRTOS/FreeRTOS.h"
#include "FreeRTOS/task.h"
#include "rtos/scheduler.h"
#include "rtos/task.h"

namespace midsun::rtos::testing {
namespace private_ {

template <typename Closure>
class OneshotTask {
 public:
  static constexpr std::chrono::milliseconds kPeriod{pdTICKS_TO_MS(1)};
  static constexpr StackSize kStackSize = StackSize::Bytes2048;
  static constexpr TaskPriority kPriority = TaskPriority::Low;
  static constexpr std::string_view kName = "OneshotTask";

  OneshotTask(Closure& closure, TaskHandle_t& handle) : closure_{closure}, handle_{handle} {}

  void Run() {
    handle_ = xTaskGetCurrentTaskHandle();
    std::invoke(closure_);
    vTaskEndScheduler();
  }

 private:
  Closure& closure_;
  TaskHandle_t& handle_;
};

}  // namespace private_

template <typename Closure>
  requires std::invocable<Closure&>
void Run(Closure&& closure) {
  TaskHandle_t handle = nullptr;
  auto executor = Scheduler{private_::OneshotTask{closure, handle}};
  executor.Start();
  // TODO(LorenzoG): Ensure neccesary.
  vTaskDelete(handle);
}

}  // namespace midsun::rtos::testing

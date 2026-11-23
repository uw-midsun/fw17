#pragma once

#include <tuple>
#include <utility>

#include "FreeRTOS/FreeRTOS.h"
#include "FreeRTOS/task.h"
#include "rtos/task.h"

namespace midsun::rtos {

template <TaskT... Tasks>
class Scheduler {
 public:
  explicit Scheduler(Tasks&&... tasks) noexcept : task_wrappers_{std::move(tasks)...} {}

  void Start() noexcept {
    std::apply([](auto&... tasks) { (tasks.Spawn(), ...); }, task_wrappers_);
    vTaskStartScheduler();
  }

 private:
  std::tuple<TaskWrapper<Tasks>...> task_wrappers_;
};

}  // namespace midsun::rtos

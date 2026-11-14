#pragma once

#include <algorithm>
#include <array>
#include <chrono>
#include <concepts>
#include <cstddef>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <utility>

#include "FreeRTOS/FreeRTOS.h"
#include "FreeRTOS/task.h"

namespace midsun::rtos {

enum class TaskPriority : std::size_t {
  Idle = 0,
  Lowest = 1,
  Low = 2,
  Medium = 3,
  High = 4,
  Highest = 5,
};
static_assert(configMAX_PRIORITIES == 5);

template <typename T>
concept TaskT = requires(T task) {
  { T::kPeriod } -> std::same_as<const std::chrono::milliseconds&>;
  { T::kStackSize } -> std::same_as<const std::size_t&>;
  { T::kPriority } -> std::same_as<const TaskPriority&>;
  // TODO(LorenzoG); String literal utility.
  { T::kName } -> std::same_as<const std::string_view&>;
  { task.Init() } -> std::same_as<void>;
  { task.Run() } -> std::same_as<void>;
};

template <TaskT Task>
class TaskWrapper {
 public:
  // TODO(LorenzoG); Figure out if copying is better than in-place constructing each task wrapper.
  explicit TaskWrapper(Task&& task) noexcept : task_object_{std::move(task)} {}

  void Spawn() noexcept {
    static constexpr auto kNameSize = Task::kName.size();

    // SAFTEY: FreeRTOS copies the name data into the task handle during task creation, so function local data is ok.
    std::array<char, kNameSize + 1> task_name{};
    std::ranges::copy_n(Task::kName.begin(), kNameSize, task_name.begin());
    task_name[kNameSize] = '\0';

    // TODO(LorenzoG): Handle this error.
    std::ignore =
        xTaskCreateStatic(&RunTask, task_name.data(), task_stack_.size(), &this->task_object_,
                          static_cast<UBaseType_t>(Task::kPriority), task_stack_.data(), &task_control_block_);
  }

 private:
  static void RunTask(void* params) noexcept {
    auto& task = *static_cast<Task*>(params);
    task.Init();

    while(true) {
      task.Run();
      vTaskDelay(pdMS_TO_TICKS(task.kPeriod.count()));
    }
  }

  std::array<StackType_t, Task::kStackSize> task_stack_{};
  StaticTask_t task_control_block_{};
  Task task_object_;
};

}  // namespace midsun::rtos

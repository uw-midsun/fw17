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
namespace private_ {

template <std::size_t kBytes>
consteval std::size_t StackSizeFromBytes() {
  static_assert(kBytes % sizeof(StackType_t) == 0,
                "Task stack size in bytes must be a multiple of sizeof(StackType_t)");
  return kBytes / sizeof(StackType_t);
}

}  // namespace detail



enum class TaskPriority : std::size_t {
  Idle = 0,
  Lowest = 1,
  Low = 2,
  Medium = 3,
  High = 4,
  Highest = 5,
};
static_assert(configMAX_PRIORITIES == 5);

enum class StackSize : std::size_t {
  Bytes512 =  private_::StackSizeFromBytes<512>(),
  Bytes1024 = private_::StackSizeFromBytes<1024>(),
  Bytes2048 = private_::StackSizeFromBytes<2048>(),
};

template <typename T>
concept TaskT = requires(T task) {
  { T::kPeriod } -> std::same_as<const std::chrono::milliseconds&>;
  requires (T::kPeriod.count() > 0);
  { T::kStackSize } -> std::same_as<const StackSize&>;
  { T::kPriority } -> std::same_as<const TaskPriority&>;
  // TODO(LorenzoG); String literal utility.
  { T::kName } -> std::same_as<const std::string_view&>;
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
  static constexpr TickType_t kPeriodTicks = pdMS_TO_TICKS(Task::kPeriod.count());
  static_assert(kPeriodTicks >= 1);

  static void RunTask(void* params) noexcept {
    auto& task = *static_cast<Task*>(params);
    auto last_wake = xTaskGetTickCount();
    while(true) {
      task.Run();
      // TODO(LorenzoG): Figure out if this should be DelayUntil or Wait for a full period.
      // TODO(LorenzoG): Add like a watchdog / overrun handling ability.
      std::ignore = xTaskDelayUntil(&last_wake, kPeriodTicks);
    }
  }

  static constexpr std::size_t kStackDepth = static_cast<std::size_t>(Task::kStackSize);
  std::array<StackType_t, kStackDepth> task_stack_{};
  StaticTask_t task_control_block_{};
  Task task_object_;
};

}  // namespace midsun::rtos

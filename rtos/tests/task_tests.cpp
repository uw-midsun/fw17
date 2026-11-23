#include "rtos/task.h"

#include <chrono>
#include <cstddef>
#include <string_view>

#include "gtest/gtest.h"

namespace midsun::rtos {
namespace {

struct TestTask {
  static constexpr std::chrono::milliseconds kPeriod{10};
  static constexpr StackSize kStackSize = StackSize::Bytes512;
  static constexpr TaskPriority kPriority = TaskPriority::Low;
  static constexpr std::string_view kName = "TestTask";

  explicit TestTask(std::size_t& run_count) : run_count_{run_count} {}

  void Run() {
    const auto now = std::chrono::milliseconds{pdTICKS_TO_MS(xTaskGetTickCount())};
    if (++run_count_ == 1U) {
      first_run_time_ = now;
      return;
    }

    const auto elapsed = now - first_run_time_;
    EXPECT_EQ(run_count_, 2U);
    EXPECT_GE(elapsed, kPeriod);
    EXPECT_LE(elapsed, kPeriod + std::chrono::milliseconds{1});
    EXPECT_EQ(std::string_view{pcTaskGetName(nullptr)}, kName);
    EXPECT_EQ(uxTaskPriorityGet(nullptr), static_cast<UBaseType_t>(kPriority));
    vTaskEndScheduler();
  }

 private:
  std::size_t& run_count_;
  std::chrono::milliseconds first_run_time_{};
};

}  // namespace

TEST(TaskWrapperTest, Integration) {
  static auto run_count = std::size_t{0U};
  static auto task = TaskWrapper{TestTask{run_count}};
  task.Spawn();
  vTaskStartScheduler();
}

}  // namespace midsun::rtos

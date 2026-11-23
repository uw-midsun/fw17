#include <chrono>
#include <cstddef>
#include <string_view>

#include "FreeRTOS/FreeRTOS.h"
#include "FreeRTOS/task.h"
#include "gtest/gtest.h"
#include "rtos/scheduler.h"
#include "rtos/task.h"

namespace midsun::rtos {
namespace {

struct CallCounts {
  std::size_t fast = 0;
  std::size_t medium = 0;
  std::size_t slow = 0;
};

struct CountingTask {
  static constexpr StackSize kStackSize = StackSize::Bytes512;
  static constexpr TaskPriority kPriority = TaskPriority::Low;

  explicit CountingTask(std::size_t& calls) : calls{calls} {}

  void Run() { calls++; }

  std::size_t& calls;
};

// TODO(LorenzoG): Add string literal library.
struct FastTask : CountingTask {
  static constexpr std::string_view kName = "FastTask";
  static constexpr std::chrono::milliseconds kPeriod{40};

  using CountingTask::CountingTask;
};

struct MediumTask : CountingTask {
  static constexpr std::string_view kName = "MediumTask";
  static constexpr std::chrono::milliseconds kPeriod{60};

  using CountingTask::CountingTask;
};

struct SlowTask : CountingTask {
  static constexpr std::string_view kName = "SlowTask";
  static constexpr std::chrono::milliseconds kPeriod{140};

  using CountingTask::CountingTask;

  void Run() {
    CountingTask::Run();
    if(calls == 2) {
      vTaskEndScheduler();
    }
  }
};

}  // namespace

TEST(SchedulerTest, RunsThreeTasksAtTheirOwnPeriodsUntilSlowestStopsIt) {
  static constinit auto calls = CallCounts{};
  static auto executor = Scheduler{
      FastTask{calls.fast},
      MediumTask{calls.medium},
      SlowTask{calls.slow},
  };
  executor.Start();

  EXPECT_GE(calls.fast, 3U);
  EXPECT_GE(calls.medium, 2U);
  EXPECT_EQ(calls.slow, 2U);
}

}  // namespace midsun::rtos

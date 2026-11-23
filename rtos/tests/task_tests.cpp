#include "rtos/task.h"

#include <chrono>
#include <string_view>

#include "gtest/gtest.h"

namespace midsun::rtos {
namespace {

struct TestTask {
  static constexpr std::chrono::milliseconds kPeriod{10};
  static constexpr StackSize kStackSize = StackSize::Bytes512;
  static constexpr TaskPriority kPriority = TaskPriority::Low;
  static constexpr std::string_view kName = "TestTask";

  void Run() {}
};

}  // namespace

// TODO(LorenzoG): To improve coverage.
TEST(TaskWrapperTest, ConstructsFromTestTask) {
  EXPECT_NO_THROW(TaskWrapper{TestTask{}});
}

}  // namespace midsun::rtos

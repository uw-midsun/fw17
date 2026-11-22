#include "rtos/task.h"

#include <chrono>
#include <cstddef>
#include <string_view>

#include "gtest/gtest.h"

namespace midsun::rtos {
namespace {

struct TestTask {
  static constexpr std::chrono::milliseconds kPeriod{10};
  static constexpr std::size_t kStackSize = 256;
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

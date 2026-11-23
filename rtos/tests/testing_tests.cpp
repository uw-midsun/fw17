#include "FreeRTOS/FreeRTOS.h"
#include "FreeRTOS/semphr.h"
#include "FreeRTOS/task.h"
#include "gtest/gtest.h"
#include "rtos/testing.h"

namespace midsun::rtos::testing {
namespace {

TEST(TestRunOnce, RunsOnce) {
  int calls = 0;
  const auto closure = [&] { calls++; };

  rtos::testing::Run(closure);
  EXPECT_EQ(calls, 1);
  rtos::testing::Run(closure);
  EXPECT_EQ(calls, 2);
}

}  // namespace
}  // namespace midsun::rtos::testing

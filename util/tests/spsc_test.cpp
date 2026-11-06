#include "util/spsc.h"

#include <gtest/gtest.h>

#include <cstddef>

namespace midsun::util::spsc {
namespace {

using TestQueue = Queue<std::byte, 32U>;

}

TEST(SpscQueue, BasicPushPop) {
  constexpr auto kTestData = std::byte{0x67};

  auto queue = TestQueue{};
  ASSERT_TRUE(queue.Push(kTestData));

  const auto maybe_data = queue.Pop();
  ASSERT_TRUE(maybe_data.has_value());
  ASSERT_EQ(maybe_data.value(), kTestData);
}

}  // namespace midsun::util::spsc

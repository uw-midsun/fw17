#include "util/spsc.h"

#include <gtest/gtest.h>

#include <cstddef>

namespace midsun::util::spsc {
namespace {

using TestQueue = Queue<std::byte, 32U>;

}

TEST(SpscQueue, BasicPushPop) {
  constexpr auto kTestData = std::byte{0x67};
  constexpr auto kTestData2 = std::array{std::byte{0x6}, std::byte{0x7}};

  auto queue = TestQueue{};
  ASSERT_TRUE(queue.Push(kTestData));

  const auto maybe_data = queue.Pop();
  ASSERT_TRUE(maybe_data.has_value());
  ASSERT_EQ(maybe_data.value(), kTestData);
}

}  // namespace midsun::util::spsc

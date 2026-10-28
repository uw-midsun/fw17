#include "util/spsc.h"

#include <gtest/gtest.h>

#include <cstddef>

namespace midsun::util::spsc {
namespace {
using TestQueue = Queue<std::byte, 32U>;
}

}  // namespace midsun::util::spsc

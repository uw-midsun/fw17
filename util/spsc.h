#pragma once

#include <array>
#include <atomic>
#include <bit>
#include <cstddef>
#include <optional>
#include <span>
#include <type_traits>

namespace midsun::util::spsc {

template <typename T, std::size_t kSize>
class Queue {
  static_assert(std::is_trivially_constructible_v<T>,
                "T must be trivially constructable to be placed into std::array.");
  static_assert(std::has_single_bit(kSize), "kSize must be a power of two for running counter design.");

 public:
  Queue() noexcept : data_{}, head_{0U}, phead_{0U}, tail_{0U}, ctail_{0U} {}

  bool Push(const std::span<const T> data) noexcept {
    const auto& data_size = data.size();
    const auto back = tail_.load(std::memory_order_relaxed);

    if((phead_ + kSize - back) < data_size) {
      phead_ = head_.load(std::memory_order_acquire);
      if((phead_ + kSize - back) < data_size) {
        return false;
      }
    }

    // TODO(LorenzoG): Maybe optimize into two calls to a memcpy wrapper.
    for(auto i = std::size_t{0U}; i < data_size; i++) {
      data_[(i + back) % kSize] = data[i];
    }

    tail_.store(back + data_size, std::memory_order_release);
    return true;
  }

  bool Push(const T& t) noexcept { return Push(std::span{&t, 1U}); }

  bool Pop(std::span<T> data) noexcept {
    const auto data_size = data.size();
    const auto front = head_.load(std::memory_order_relaxed);

    if((ctail_ - front) < data_size) {
      ctail_ = tail_.load(std::memory_order_acquire);
      if(ctail_ - front < data_size) {
        return false;
      }
    }

    // TODO(LorenzoG): Maybe optimize into two calls to a memcpy wrapper.
    for(auto i = std::size_t{0U}; i < data_size; i++) {
      data[i] = data_[(i + front) % kSize];
    }

    head_.store(front + data_size, std::memory_order_release);
    return true;
  }

  std::optional<T> Pop() noexcept {
    T t{};
    if(!Pop(std::span{&t, 1U})) {
      return std::nullopt;
    }

    return t;
  }

  // TODO(LorenzoG): Validate this.
  // bool Full() const noexcept {
  //   const auto back = tail_.load(std::memory_order_relaxed);
  //   const auto h = head_.load(std::memory_order_acquire);

  //   return (h + kSize - back) == 0U;
  // }

  // // TODO(LorenzoG): Validate this.
  // bool Empty() const noexcept {
  //   const auto front = head_.load(std::memory_order_relaxed);
  //   const auto t = tail_.load(std::memory_order_acquire);

  //   return (t - front) == 0U;
  // }

 private:
  std::array<T, kSize> data_;
  std::atomic_size_t head_;  // Reader owned, writer aq.
  std::size_t phead_;        // Writer owned cache of head.
  std::atomic_size_t tail_;  // Writer owned, reader aq.
  std::size_t ctail_;        // Reader owned cache of head.
};

}  // namespace midsun::util::spsc

#pragma once

#include <tuple>
#include <type_traits>

namespace midsun::core::executor {

template <typename T>
concept Task = requires {
  { T::kName } -> std::is_convertible_to<std::string_view>;
  { std::declval<T>.Run() } -> std::same_as<void>;
};

template <Task... Tasks>
class Executor {
 public:
  using TasksTuple = std::tuple<Tasks...>;

  Executor(Tasks... tasks) : tasks_{tasks...} {}

  void Run() {
    std::apply([](const auto&... tasks) { (tasks.Run(), ...); }, tasks_);
  }

  Executor(const Executor&) = delete;
  Executor(const Executor&&) = delete;
  Executor& operator=(const Executor&) = delete;
  Executor& operator=(const Executor&&) = delete;

 private:
  TasksTuple tasks_;
};

}  // namespace midsun::core::executor

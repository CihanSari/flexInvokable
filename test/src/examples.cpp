#include <csari/flexInvokable.hpp>
#include <iostream>
#include <vector>

// void()
auto voidReturn() {
  auto showFive = [mu = std::make_unique<int>(5)] {
    std::cout << "Five is " << *mu << '\n';
  };
  auto showSix = [mu = std::make_unique<int>(6)] {
    std::cout << "Six is " << *mu << '\n';
  };
  std::vector<csari::flexInvokable<void()>> res;
  res.emplace_back(std::move(showFive));
  res.emplace_back(std::move(showSix));
  return res;
}

// bool()
auto trueReturn() {
  auto isFive = [mu = std::make_unique<int>(5)] { return *mu == 5; };
  auto isSix = [mu = std::make_unique<int>(6)] { return *mu == 6; };
  std::vector<csari::flexInvokable<bool()>> res;
  res.emplace_back(std::move(isFive));
  res.emplace_back(std::move(isSix));
  return res;
}

// bool(int)
auto contentIsEqualReturn() {
  auto isEqual1 = [mu = std::make_unique<int>(5)](int const in) {
    return *mu == in;
  };
  auto isEqual2 = [mu = std::make_unique<int>(6)](int const in) {
    return *mu == in;
  };
  std::vector<csari::flexInvokable<bool(int)>> res;
  res.emplace_back(std::move(isEqual1));
  res.emplace_back(std::move(isEqual2));
  return res;
}

int examples() {
  for (auto &toInvoke : voidReturn()) {
    std::move(toInvoke)();
  }
  for (auto &toInvoke : trueReturn()) {
    bool res = std::move(toInvoke)();
    if (!res) {
      // Fail the example
      return 1;
    }
  }
  auto contentCheckers = contentIsEqualReturn();
  auto it = contentCheckers.begin();
  if (!std::move(*it)(5)) {
    // Fail the example
    return 1;
  }
  ++it;
  if (!std::move(*it)(6)) {
    // Fail the example
    return 1;
  }
  // Example finished
  return 0;
}

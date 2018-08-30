#include <iostream>
#include <vector>
#include "../include/flexInvokable.hpp"

// void()
auto voidReturn() {
  auto fiveUniqueContainer = std::make_unique<int>(5);
  auto sixUniqueContainer = std::make_unique<int>(6);
  auto showFive = [mu = std::move(fiveUniqueContainer)] {
    std::cout << "Five is " << *mu.get() << '\n';
  };
  auto showSix = [mu = std::move(sixUniqueContainer)] {
    std::cout << "Six is " << *mu.get() << '\n';
  };
  std::vector<csari::flexInvokable<void()>> res;
  res.emplace_back(std::move(showFive));
  res.emplace_back(std::move(showSix));
  return res;
}

// bool()
auto trueReturn() {
  auto fiveUniqueContainer = std::make_unique<int>(5);
  auto sixUniqueContainer = std::make_unique<int>(6);
  auto isFive = [mu = std::move(fiveUniqueContainer)] {
    return *mu.get() == 5;
  };
  auto isSix = [mu = std::move(sixUniqueContainer)] { return *mu.get() == 6; };
  std::vector<csari::flexInvokable<bool()>> res;
  res.emplace_back(std::move(isFive));
  res.emplace_back(std::move(isSix));
  return res;
}

// bool(int)
auto contentIsEqualReturn() {
  auto fiveUniqueContainer = std::make_unique<int>(5);
  auto sixUniqueContainer = std::make_unique<int>(6);
  auto isEqual1 = [mu = std::move(fiveUniqueContainer)](int in) {
    return *mu.get() == in;
  };
  auto isEqual2 = [mu = std::move(sixUniqueContainer)](int in) {
    return *mu.get() == in;
  };
  std::vector<csari::flexInvokable<bool(int)>> res;
  res.emplace_back(std::move(isEqual1));
  res.emplace_back(std::move(isEqual2));
  return res;
}

int main() {
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

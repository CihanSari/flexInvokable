#include "gtest/gtest.h"

#include <vector>
#include <csari/flexInvokable.hpp>
#include <stdexcept>

TEST(UnitTests, EncapsulationTests) {
  // void()
  auto voidReturn = [] {
    auto fiveUniqueContainer = std::make_unique<int>(5);
    auto sixUniqueContainer = std::make_unique<int>(6);
    auto showFive = [mu = std::move(fiveUniqueContainer)] {
      if (*mu.get() != 5) {
        throw(std::runtime_error{"Unexpected test error!"});
      }
    };
    auto showSix = [mu = std::move(sixUniqueContainer)] {
      if (*mu.get() != 6) {
        throw(std::runtime_error{"Unexpected test error!"});
      }
    };
    std::vector<csari::flexInvokable<void()>> res;
    res.emplace_back(std::move(showFive));
    res.emplace_back(std::move(showSix));
    return res;
  };

  for (auto &toInvoke : voidReturn()) {
    std::move(toInvoke)();
  }
}

TEST(UnitTests, ReturnValueTests) {
  // int()
  auto returnValue = [] {
    std::vector<csari::flexInvokable<int()>> res;

    auto uniqueContainer = std::make_unique<int>(5);
    auto isFive = [mu = std::move(uniqueContainer)] { return *mu.get(); };
    res.emplace_back(std::move(isFive));

    uniqueContainer.reset();
    uniqueContainer = std::make_unique<int>(6);
    auto isSix = [mu = std::move(uniqueContainer)] { return *mu.get(); };
    res.emplace_back(std::move(isSix));

    return res;
  };

  auto contentCheckers = returnValue();
  auto it = contentCheckers.begin();
  EXPECT_EQ(std::move (*it)(), 5);
  ++it;
  EXPECT_EQ(std::move (*it)(), 6);
}

TEST(UnitTests, ArgumentTests) {
  // bool(int)
  auto contentIsEqualReturn = [] {
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
  };
  auto contentCheckers = contentIsEqualReturn();
  auto it = contentCheckers.begin();
  EXPECT_TRUE(std::move (*it)(5));
  ++it;
  EXPECT_TRUE(std::move (*it)(6));
}
#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include <csari/flexInvokable.hpp>
#include <stdexcept>
#include <vector>

TEST_CASE("EncapsulationTests") {
  // void()
  auto voidReturn = [] {
    auto check5 = [mu = std::make_unique<int>(5)] { REQUIRE(*mu == 5); };
    auto check6 = [mu = std::make_unique<int>(6)] { REQUIRE(*mu == 6); };
    std::vector<csari::flexInvokable<void()>> res;
    res.emplace_back(std::move(check5));
    res.emplace_back(std::move(check6));
    return res;
  };

  for (auto &toInvoke : voidReturn()) {
    std::move(toInvoke)();
  }
}

TEST_CASE("ReturnValueTests") {
  // int()
  auto returnValue = [] {
    std::vector<csari::flexInvokable<int()>> res;
    res.emplace_back([mu = std::make_unique<int>(5)] { return *mu; });
    res.emplace_back([mu = std::make_unique<int>(6)] { return *mu; });
    return res;
  };

  auto contentCheckers = returnValue();
  auto it = contentCheckers.begin();
  REQUIRE(std::move(*it)() == 5);
  ++it;
  REQUIRE(std::move(*it)() == 6);
}

TEST_CASE("ArgumentTests") {
  // bool(int)
  auto contentIsEqualReturn = [] {
    auto isEqual1 = [mu = std::make_unique<int>(5)](int in) {
      return *mu == in;
    };
    auto isEqual2 = [mu = std::make_unique<int>(6)](int in) {
      return *mu == in;
    };
    std::vector<csari::flexInvokable<bool(int)>> res;
    res.emplace_back(std::move(isEqual1));
    res.emplace_back(std::move(isEqual2));
    return res;
  };
  auto contentCheckers = contentIsEqualReturn();
  auto it = contentCheckers.begin();
  REQUIRE(std::move(*it)(5));
  ++it;
  REQUIRE(std::move(*it)(6));
}

TEST_CASE("ThrowTests") {
  // void()
  struct fiveThrow : std::runtime_error {
    using runtime_error::runtime_error;
  };
  auto throw1 = [mu = std::make_unique<std::string>("five")] {
    throw fiveThrow{*mu};
  };
  struct sixThrow : std::runtime_error {
    using runtime_error::runtime_error;
  };
  auto throw2 = [mu = std::make_unique<std::string>("six")] {
    throw sixThrow{*mu};
  };
  REQUIRE_THROWS_AS(throw1(), fiveThrow);
  REQUIRE_THROWS_AS(throw2(), sixThrow);
}

int examples();
TEST_CASE("Examples") { REQUIRE(examples() == 0); }
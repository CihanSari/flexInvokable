#include <memory>
#include <stdexcept>
#include <type_traits>

namespace csari {
template <class Sig>
struct flexInvokable;
template <class R, class... Args>
struct flexInvokable<R(Args...)> final {
 private:
  template <class F>
  struct parser {
    using D = F;
    static auto const IsConvertible =
        std::is_convertible<std::invoke_result_t<F, Args...>, R>::value;
    static auto const IsVoid = std::is_same_v<R, void>;
    static auto const IsSelf = std::is_same_v<D, flexInvokable>;
    static auto const IsValidFunction = IsConvertible || IsVoid || !IsSelf;
    using EnableIfValid = std::enable_if_t<IsValidFunction, bool>;
  };

 public:
  // can be default ctored and moved:
  flexInvokable() = default;
  flexInvokable(flexInvokable &&) noexcept = default;
  flexInvokable &operator=(flexInvokable &&) noexcept = default;
  flexInvokable(flexInvokable const &) = delete;
  flexInvokable &operator=(flexInvokable const &) = delete;
  ~flexInvokable() = default;

  // implicitly create from a type that can be invoked
  template <class F, class P = parser<std::decay_t<F>>,
            typename P::EnableIfValid = true>
  flexInvokable(F &&f) : flexInvokable{P{}, std::forward<F>(f)} {}

  // Construct using the parser tag type:
  template <class F, class... FArgs>
  flexInvokable(parser<F>, FArgs &&...args)
      : ptr{std::make_shared<F>(std::forward<FArgs>(args)...)},
        invoke{[](void *pf, Args... args) -> R {
          return (*(F *)pf)(std::forward<Args>(args)...);
        }} {}

  auto operator()(Args... args) -> decltype(auto) {
    try {
      if constexpr (std::is_same_v<R, void>) {
        invoke(ptr.get(), std::forward<Args>(args)...);
        reset();
      } else {
        auto ret = invoke(ptr.get(), std::forward<Args>(args)...);
        reset();
        return ret;
      }
    } catch (std::exception const &e) {
      reset();
      throw e;
    }
  }

  void reset() {
    invoke = nullptr;
    ptr.reset();
  }

  // test if it is non-empty:
  explicit operator bool() const { return (bool)ptr; }

 private:
  // storage. shared_ptr to type-erase and clean-up.
  std::shared_ptr<void> ptr{};
  R (*invoke)(void *, Args...) = nullptr;
};
}  // namespace csari
#include <type_traits>
#include <memory>
namespace csari
{
template <class Sig>
struct flexInvokable;

namespace flexInvokableInternal
{
template <class T>
struct emplace_as
{
};
} // namespace flexInvokableInternal
template <class R, class... Args>
struct flexInvokable<R(Args...)>
{
  // can be default ctored and moved:
  flexInvokable() = default;
  flexInvokable(flexInvokable &&) = default;
  flexInvokable &operator=(flexInvokable &&) = default;

  // implicitly create from a type that can be compatibly invoked
  // and isn't a fire_once itself
  template <class F,
            std::enable_if_t<!std::is_same<std::decay_t<F>, flexInvokable>{},
                             int> = 0,
            std::enable_if_t<std::is_convertible<
#if _HAS_CXX17
                                 std::invoke_result_t<F, Args...>
#else
                                 std::result_of_t<std::decay_t<F> &(Args...)>
#endif
                                 ,
                                 R>{} ||
                                 std::is_same<R, void>{},
                             int> = 0>

  flexInvokable(F &&f)
      : flexInvokable(flexInvokableInternal::emplace_as<std::decay_t<F>>{},
                      std::forward<F>(f))
  {
  }
  // emplacement construct using the emplace_as tag type:
  template <class F, class... FArgs>
  flexInvokable(flexInvokableInternal::emplace_as<F>, FArgs &&... fargs)
  {
    rebind<F>(std::forward<FArgs>(fargs)...);
  }
  // invoke in the case where R is not void:
  template <class R2 = R, std::enable_if_t<!std::is_same<R2, void>{}, int> = 0>
  R2 operator()(Args... args) &&
  {
    try
    {
      R2 ret = invoke(ptr.get(), std::forward<Args>(args)...);
      clear();
      return ret;
    }
    catch (std::runtime_error e)
    {
      clear();
      throw;
    }
  }
  // invoke in the case where R is void:
  template <class R2 = R, std::enable_if_t<std::is_same<R2, void>{}, int> = 0>
  R2 operator()(Args... args) &&
  {
    try
    {
      invoke(ptr.get(), std::forward<Args>(args)...);
      clear();
    }
    catch (...)
    {
      clear();
      throw;
    }
  }

  // empty the fire_once:
  void clear()
  {
    invoke = nullptr;
    ptr.reset();
  }

  // test if it is non-empty:
  explicit operator bool() const { return (bool)ptr; }

  // change what the flexInvokable contains:
  template <class F, class... FArgs>
  void rebind(FArgs &&... fargs)
  {
    clear();
    auto pf = std::make_unique<F>(std::forward<FArgs>(fargs)...);
    invoke = [](void *pf, Args... args) -> R {
      return (*(F *)pf)(std::forward<Args>(args)...);
    };
    ptr = {pf.release(), [](void *pf) { delete (F *)(pf); }};
  }

private:
  // storage.  A unique pointer with deleter
  // and an invoker function pointer:
  std::unique_ptr<void, void (*)(void *)> ptr{nullptr, [](void *) {}};
  R (*invoke)
  (void *, Args...) = nullptr;
};
} // namespace csari
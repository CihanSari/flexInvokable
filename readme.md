# Flexible invokable for C++17

Single header container for lambdas with (or without) move-only encapsulation that can be invoked only once.

What are lambdas? With a huge simplification, I think of them as somehow glorified structs. For example instead of:
```
int main() {
  int res = 0;

  int inc = 1;
  struct {
    int &inc;
    bool operator()(int &parameter) const {
      parameter += inc;
      return true;
    }
  } fakeLambda{inc};
  fakeLambda(res);

  return res;
}
```

Today you can write:
```
int main() {
  int res = 0;

  int inc = 1;
  auto lambda = [&inc](int &parameter) {
    parameter += inc;
    return true;
  };
  lambda(res);

  return res;
}
```

I won't get into details of why lambdas are great (they are!). The topic of this repository is to address the need to store these lambdas in containers (such as vectors). The generic way you are expected to do this is in [std::function](https://en.cppreference.com/w/cpp/utility/functional/function). However, `std::function` requires the encapsulated contents to be copy constructable. This is easy to imagine from the above struct: encapsulations sometimes cause issue! If you have members that are not copiable, then you cannot use `std::function` even though contents are movable. This is the need we are trying to address in `flexInvokable`. You can store your lambdas in move-only containers.

## Example
Example is shown in `example.cpp`.

## How to use
Add flexInvokable.hpp to your project.


## Source
Originally found as an answer to the request in [stackoverflow](https://stackoverflow.com/questions/25421346/how-to-create-an-stdfunction-from-a-move-capturing-lambda-expression). 

I had to make slight changes from [source demo](http://coliru.stacked-crooked.com/a/1cfdcd0491686fe1) 
```
void(*invoke)(void*, Args...) = nullptr; 
```
to
```
R(*invoke)(void*, Args...) = nullptr; 
```
Moreover, 
```
std::result_of_t<std::decay_t<F>&(Args...)>
```
is getting deprecated, and therefore replaced to
```
#if _HAS_CXX17
    std::invoke_result_t<F, Args...>
#else
    std::result_of_t<std::decay_t<F>&(Args...)>
#endif
```

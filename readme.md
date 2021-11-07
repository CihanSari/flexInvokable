# Flexible invokable for C++17

Single header container for lambdas with (or without) move-only encapsulation that can be invoked only once.

The topic of this repository is to address the need to store lambdas in containers (such as vectors). The generic way you are expected to do this is in [std::function](https://en.cppreference.com/w/cpp/utility/functional/function). However, `std::function` requires the encapsulated contents to be copy constructable. This is easy to imagine from the above struct: encapsulations sometimes cause issue! If you have members that are not copiable, then you cannot use `std::function` even though contents are movable. This is the need we are trying to address in `flexInvokable`. You can store your lambdas in move-only containers.

[![vah_CI](https://github.com/CihanSari/flexInvokable/actions/workflows/main.yml/badge.svg)](https://github.com/CihanSari/flexInvokable/actions/workflows/main.yml)
[![CodeQL](https://github.com/CihanSari/flexInvokable/actions/workflows/codeql-analysis.yml/badge.svg)](https://github.com/CihanSari/flexInvokable/actions/workflows/codeql-analysis.yml)

## Example
Example is shown in `test/src/example.cpp`.

## How to use
Add flexInvokable/include/flexInvokable.hpp to your project.


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

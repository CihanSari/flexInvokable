# Move only invokable
Single header invokable to store noncopyable structures and execute them later on c++17.

## Source
Originally found as an answer to the question in [stackoverflow](https://stackoverflow.com/questions/25421346/how-to-create-an-stdfunction-from-a-move-capturing-lambda-expression). 

I had to change one line from [source demo](http://coliru.stacked-crooked.com/a/1cfdcd0491686fe1) 
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
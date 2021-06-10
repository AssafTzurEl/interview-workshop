# Lazy Fibonacci

Implement the following function:
```C++
unsigned long long Fibonacci(size_t n);
```

Use lazy implementation: The library stores calculated values in a cache. When asked to calculate Fibonacci(3), for example, the function checks whether the value is already stored in the cache. If it is, it returns that number; if it isn't, the function will calculate that number and store it in the cache for future use.

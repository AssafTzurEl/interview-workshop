#include "Fibonacci.h"

Fibonacci::fsize_t Fibonacci::Get(size_t n)
{
    if (s_cache.size() <= n)
    {
        // n hasn't been requested before, build it:
        auto fib_n = Get(n - 1) + Get(n - 2);
        // then populate the cache, to allow laziness in the future:
        s_cache.push_back(fib_n);
    }
    // else - n is already in. Either way, we now have it cached:
    return s_cache[n];
}

// Don't forget to initialize your statics!
// Starting with two values, to prevent the call to "Get(n-2)" above from being negative.
std::vector<Fibonacci::fsize_t> Fibonacci::s_cache{ 0, 1 };
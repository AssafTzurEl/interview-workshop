#pragma once
// Or use the #ifndef _FIBONACCI_H_ + #define _FIBONACCI_H_ + #undef trick

#include <vector>

// We need a function, but also a cache. We could have done this with a global function and a static variable,
// but this is not the OO way. Instead, we'll encapsulate everything in a class, and since there's no real state here,
// we'll make everything static.
class Fibonacci
{
public:
    // Fibonacci numbers get large very quickly, so we're using the largest integral type possible.
    // Also, Fibonacci numbers are never negative, so we're using unsigned.
    // If a future version of the C++ standard adds an 'unsigned very very long long' type,
    // we can just change the typedef.
    typedef unsigned long long fsize_t;

    // Shortest name possible, to make the call short: Fibonacci::Get(N)
    // Would love to use operator(), but it can't be static.
    // We could have declared a ctor and be able to call Fibonacci(N), but then we'd have to create a cast operator
    // that converts a Fibonacci object to fsize_t, and that would make things more complicated.
    static fsize_t Get(size_t n);

private:
    // Our database: A simple, resizeable cache:
    static std::vector<fsize_t> s_cache;
};


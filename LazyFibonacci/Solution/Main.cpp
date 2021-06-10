#include <iostream>
#include <vector>
#include "Fibonacci.h"

int main()
{
    // Using a debugger, step into the function and see it pupulating the internal cache recursively:
    auto num = Fibonacci::Get(3);

    num = Fibonacci::Get(10);
    // Using a debugger, check out Fibonacci::s_cache and see that it now contains 11 items.

    // Step into the following call and make sure it extracts an existing value
    // from the cache instead of recursing:
    num = Fibonacci::Get(7);

    return 0;
}

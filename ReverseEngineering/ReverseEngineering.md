# Reverse Engineering - solution

Let's start with identifying keywords:
```C
DWORD _(DWORD __)
{
    const DWORD ___ = !NULL;
    return __ > ___ ? _(__ - ___) + _(__ - (___ + ___)) : ___;
}
```

`DWORD` is a double-word, i.e. four bytes. It is used to denote integers (and unsigned ints), so let's try replacing all `DWORD`s with `int`s; in the worst case (if they turn out to denote pointers, for example), we'll undo this step later on.
```C
int _(int __)
{
    const int ___ = !NULL;
    return __ > ___ ? _(__ - ___) + _(__ - (___ + ___)) : ___;
}
```

All the identifiers have been replaced by underscores, so we need to change them back. The most obvious one is the "`_`" at the top: Based on the parentheses to the right and the type to the left, I'd say it's a function name. We don't know what it is, so we'll call it `f` for now:
```C
int f(int __)
{
    const int ___ = !NULL;
    return __ > ___ ? f(__ - ___) + f(__ - (___ + ___)) : ___;
}
```

The plot thicken... our function calls itself. It's a recursive function!

Back to business: The "`__`" at the top looks like a parameter - let's give it a generic parameter name:
```C
int f(int param)
{
    const int ___ = !NULL;
    return param > ___ ? f(param - ___) + f(param - (___ + ___)) : ___;
}
```

The `const`'s value is `!NULL`, which preprocesses to `!0`, which compiles to `1`. Instead of using the variable `___`, let's use the literal `1`:
```C
int f(int param)
{
    return param > 1 ? f(param - 1) + f(param - (1 + 1)) : 1;
}
```

1 + 1 is 2, so:

```C
int f(int param)
{
    return param > 1 ? f(param - 1) + f(param - 2) : 1;
}
```

By now it should be clear. Let's name it properly! (and if you really want to be optimized, you can also replace the `int`s with `unsigned int`s):
```C
int Fibonacci(int n)
{
    return n > 1 ? Fibonacci(n - 1) + Fibonacci(n -2) : 1;
}
```

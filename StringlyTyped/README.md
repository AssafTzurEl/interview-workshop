# Stringly Typed

Write the following functions:

\#	| Receive | Return | Example
---|---------|--------|--------
1  | Integral number | Its representation as string (`itoa`) | 123 => “123”
2  | Integral number | Its hex string representation         | 123 => “7B”
3  | Integral number | Its binary string representation      | 123 => “01111011”

## Notes

* Do this the hard way: Do not use `itoa`, `std::hex`, `stringstream`, etc.
* You may add base prefixes (e.g. `0x`) where applicable

## Upgrades

* Make the function add zeros according to the numeric type’s size, e.g. an int 123 will be converted to "00000123"
* Add C++11-style number spearator for bits (e.g. "1010'1010")

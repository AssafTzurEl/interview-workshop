#include <string>
#include <cassert>
#include <algorithm>
#include <limits.h>

namespace StringlyTyped
{
    // Receives any integral type: int, unsigned int, long, short, etc.
    // Classic solution: Got right-most digit using modulo 10, scan rest of number, etc.
    template <class T>
    std::string IntToString(T num)
    {
        const T DIGIT_SIZE = 10;

        // C++11 required:
        static_assert(std::is_integral<T>::value, "Integral type required.");

        auto isNegative = num < 0;

        if (isNegative)
        {
            num = -num;
        }

        std::string result;

        do
        {
            char digit = num % DIGIT_SIZE + '0';

            result += digit;
            num /= DIGIT_SIZE;
        } while (num);

        // Do not pad string with zeros, otherwise it might be parsed as octal!

        if (isNegative)
        {
            result += '-';
        }

        // Digits have been pushed from lowest to highest - reverse their ordeR:
        std::reverse(result.begin(), result.end());

        return result;
    }

    // Receives any integral type: int, unsigned int, long, short, etc.
    // Scan the number nybble by nybble
    template <class T>
    std::string IntToHexString(T num)
    {
        // C++11 required:
        static_assert(std::is_integral<T>::value, "Integral type required.");

        std::string result;

        // Go through the number's bytes:
        for (size_t count = 0; count < sizeof(T) * 2; ++count)
        {
            unsigned char digit = num & 0x0F;

            // '0'-'9', 'A'-'F'
            result += digit + (digit < 10 ? '0' : ('A' - 10));
            num >>= 4;
        }

        result += "x0"; // to be reversed shortly

        // Digits have been pushed from lowest to highest - reverse their ordeR:
        std::reverse(result.begin(), result.end());

        return result;
    }

    // Receives any integral type: int, unsigned int, long, short, etc.
    // Scan the number bit by bit - watch out for endianness!
    template <class T>
    std::string IntToBinString(const T &num)
    {
        std::string result;

        for (size_t byteCount = 0; byteCount < sizeof(T); ++byteCount)
        {
            unsigned char byte = *((unsigned char *) &num + byteCount);

            for (size_t bitCount = 0; bitCount < CHAR_BIT; ++bitCount)
            {
                result += ((byte >> bitCount) & 1) ? '1' : '0';

                if (bitCount + 1 == CHAR_BIT / 2)
                {
                    result += '\''; // insert separator after nybble (half-byte) for pretty-printing
                }
            }

            result += '\''; // insert separator after 8 bits for pretty-printing
        }

        // extra separator remaining - remove it!
        result.resize(result.size() - 1);

        result += "b0"; // to be reversed shortly

        // Digits have been pushed from lowest to highest - reverse their ordeR:
        std::reverse(result.begin(), result.end());

        return result;
    }
} // namespace StringlyTyped

int main()
{
    assert(StringlyTyped::IntToString(0) == "0");
    assert(StringlyTyped::IntToString(11) == "11");
    assert(StringlyTyped::IntToString(-2'222'222'222) == "-2222222222");

    assert(StringlyTyped::IntToHexString((int32_t) 0) == "0x00000000"); // int size is implementation-defined; be specific!
    assert(StringlyTyped::IntToHexString((int16_t) 0) == "0x0000");
    assert(StringlyTyped::IntToHexString((int64_t) 0) == "0x0000000000000000");
    assert(StringlyTyped::IntToHexString('A') == "0x41");
    assert(StringlyTyped::IntToHexString(-42) == "0xFFFFFFD6");
    assert(StringlyTyped::IntToHexString((uint32_t) 42) == "0x0000002A");

    assert(StringlyTyped::IntToBinString((int16_t) 0) == "0b0000'0000'0000'0000");
    assert(StringlyTyped::IntToBinString((uint8_t) 0x7C) == "0b0111'1100");
    assert(StringlyTyped::IntToBinString((uint8_t) -1) == "0b1111'1111");

    return 0;
}

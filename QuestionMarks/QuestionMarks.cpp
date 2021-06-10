#include <iostream>
#include <vector>
#include <string>

/*
 * @brief You can start here, but you probably want to call FindPermutation(const std::string &).
 * 
 * @param str    template string, with partial resolution of question marks
 * @param pos    index in the string to start scanning from
 * @param result collection for accumulating results
 */
void FindPermutations(std::string &str, size_t pos, std::vector<std::string> &result)
{
    pos = str.find('?', pos);

    if (pos == std::string::npos)
    {
        // End of string, no more permutations
        result.push_back(str);
        return;
    }
    // else - create permutations for current question mark, then continue with the rest of the string:
    str[pos] = '0';
    FindPermutations(str, pos + 1, result);
    str[pos] = '1';
    FindPermutations(str, pos + 1, result);
    str[pos] = '?';
}

/*
 * @brief Given a string of ones, zeros, and question marks, generate
 *        all possible replacements of the question marks with either
 *        one or zero.
 * 
 * @param  templateString the input string
 * @return a collection of all possible generated strings
 */
std::vector<std::string> FindPermutations(const std::string &templateString)
{
    std::vector<std::string> result;
    auto str(templateString); // will mutate, so make a copy

    FindPermutations(str, 0, result);

    return result;
}


int main()
{
    auto strings = FindPermutations("0?10??");

    return 0;
}

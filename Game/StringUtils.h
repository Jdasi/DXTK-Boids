#pragma once
#include <vector>

/* Namespace containing utility/helper functions associated with strings and char arrays.
 */
namespace StringUtils
{
    void split_string_by_delim(const std::string& _str, char _delim, std::vector<std::string>& _container);
    wchar_t* char_to_wchar(const char* _text);
}

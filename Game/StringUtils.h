#pragma once
#include <vector>

/* Namespace containing utility/helper functions associated with strings and char arrays.
 */
namespace StringUtils
{
    void splitStringByDelim(const std::string& _str, char _delim, std::vector<std::string>& _container);
    wchar_t* charToWChar(const char* _text);
}

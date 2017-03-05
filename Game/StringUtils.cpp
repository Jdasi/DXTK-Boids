#include "StringUtils.h"

#include <sstream>

/* Splits up a string based on the passed delimiter, and fills the passed container
 * with the results of the split.
 */
void StringUtils::split_string_by_delim(const std::string& _str, char _delim, std::vector<std::string>& _container)
{
    // Prepares the container for insertions.
    _container.clear();

    std::stringstream ss(_str);
    std::string item;

    // Fill the container with split up elements based on the delimiter.
    while (getline(ss, item, _delim))
    {
        _container.push_back(item);
    }
}


/* Helper function for converting char strings to WChar.
 */
wchar_t* StringUtils::char_to_wchar(const char* _text)
{
    size_t size = strlen(_text) + 1;
    static wchar_t* wa = NULL;

    if (wa)
    {
        delete[] wa;
        wa = NULL;
    }

    wa = new wchar_t[size];
    mbstowcs_s(nullptr, wa, size, _text, size);

    return wa;
}

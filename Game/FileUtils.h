#pragma once
#include <jsoncons/json.hpp>

using jsoncons::json;

// Namespace containing utility/helper functions associated with file I/O.
namespace FileUtils
{
	json read_json(const std::string& _file_name);
}

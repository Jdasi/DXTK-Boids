#include "FileUtils.h"

#include <fstream>

/* Opens a file with the passed file_name, reads its data into a JSON object
 * and then returns it.
 */
json FileUtils::read_json(const std::string& _file_name)
{
	std::ifstream file("../Streaming/" + _file_name + ".json");

    json file_data;
    file >> file_data;

    return file_data;
}

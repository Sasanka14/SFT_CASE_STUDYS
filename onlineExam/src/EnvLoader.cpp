#include "EnvLoader.h"
#include <fstream>
#include <iostream>
#include <cstdlib>

bool EnvLoader::load(const std::string& filepath)
{
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "⚠️ Could not open " << filepath << "\n";
        return false;
    }

    std::string line;
    int loaded = 0;

    while (std::getline(file, line))
    {
        // Skip empty lines and comments
        if (line.empty() || line[0] == '#')
            continue;

        auto pos = line.find('=');
        if (pos == std::string::npos)
            continue;

        std::string key = line.substr(0, pos);
        std::string value = line.substr(pos + 1);

        // Trim whitespace from key and value
        auto trim = [](std::string& s) {
            size_t start = s.find_first_not_of(" \t\r\n");
            size_t end = s.find_last_not_of(" \t\r\n");
            if (start == std::string::npos) { s.clear(); return; }
            s = s.substr(start, end - start + 1);
        };

        trim(key);
        trim(value);

        // Remove surrounding quotes if present
        if (value.size() >= 2 &&
            ((value.front() == '"' && value.back() == '"') ||
             (value.front() == '\'' && value.back() == '\'')))
        {
            value = value.substr(1, value.size() - 2);
        }

        if (!key.empty()) {
            setenv(key.c_str(), value.c_str(), 1);
            loaded++;
        }
    }

    return loaded > 0;
}

#ifndef ENVLOADER_H
#define ENVLOADER_H

#include <string>

namespace EnvLoader {
    // Load key=value pairs from a .env file into process environment
    bool load(const std::string& filepath);
}

#endif

#ifndef CONSOLEUI_H
#define CONSOLEUI_H

#include <string>

namespace ConsoleUI {
    void printHeader(const std::string& text);
    void printSuccess(const std::string& text);
    void printError(const std::string& text);
    void printWarning(const std::string& text);
    void printPassBadge();
    void printFailBadge();
}

#endif
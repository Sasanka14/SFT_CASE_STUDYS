#include "ConsoleUI.h"
#include <iostream>

namespace {
    const std::string RESET = "\033[0m";
    const std::string CYAN = "\033[36m";
    const std::string GREEN = "\033[32m";
    const std::string RED = "\033[31m";
    const std::string YELLOW = "\033[33m";
    const std::string BOLD = "\033[1m";
}

void ConsoleUI::printHeader(const std::string& text) {
    std::cout << CYAN << text << RESET << "\n";
}

void ConsoleUI::printSuccess(const std::string& text) {
    std::cout << GREEN << text << RESET << "\n";
}

void ConsoleUI::printError(const std::string& text) {
    std::cout << RED << text << RESET << "\n";
}

void ConsoleUI::printWarning(const std::string& text) {
    std::cout << YELLOW << BOLD << text << RESET << "\n";
}

void ConsoleUI::printPassBadge() {
    std::cout << GREEN << BOLD 
              << "╔════════════════════════════════════════╗\n"
              << "║              ✅ PASSED! ✅              ║\n"
              << "║      Congratulations on your success!  ║\n"
              << "╚════════════════════════════════════════╝"
              << RESET << "\n";
}

void ConsoleUI::printFailBadge() {
    std::cout << RED << BOLD
              << "╔════════════════════════════════════════╗\n"
              << "║              ❌ FAILED ❌              ║\n"
              << "║         Please review and retry!       ║\n"
              << "╚════════════════════════════════════════╝"
              << RESET << "\n";
}
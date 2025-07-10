#ifndef PRISM_STYLE_HPP_
#define PRISM_STYLE_HPP_

#include "prism_export.h"

#include <iostream>
#include <string>

namespace Prism {

/**
 * @namespace Style
 * @brief Provides constants for colored console output.
 *
 * This namespace centralizes ANSI escape codes for styling text in the terminal,
 * ensuring a consistent look and feel for all engine logs.
 */
namespace Style {

// --- Basic Colors ---
const std::string RESET = "\033[0m";
const std::string YELLOW = "\033[0;33m";
const std::string GREEN = "\033[0;32m";
const std::string CYAN = "\033[0;36m";
const std::string GRAY = "\033[0;90m";
const std::string RED = "\033[0;31m";

// --- Bold Variants ---
const std::string BOLD_CYAN = "\033[1;36m";
const std::string BOLD_GREEN = "\033[1;32m";
const std::string BOLD_RED = "\033[1;31m";
const std::string BOLD_YELLOW = "\033[1;33m";

/**
 * @brief Logs a formatted informational message to std::clog.
 * @param message The message to display.
 */
inline void logInfo(const std::string& message) {
    std::clog << YELLOW << "[INFO] " << RESET << message << std::endl;
}

/**
 * @brief Logs a formatted completion message to std::clog.
 * @param message The message to display.
 */
inline void logDone(const std::string& message) {
    std::clog << GREEN << "[DONE] " << RESET << message << std::endl;
}

/**
 * @brief Logs a formatted error message to std::cerr.
 * @param message The error message to display.
 */
inline void logError(const std::string& message) {
    std::cerr << BOLD_RED << "[ERROR] " << RESET << RED << message << RESET << std::endl;
}

/**
 * @brief Logs a formatted warning message to std::clog.
 * @param message The warning message to display.
 */
inline void logWarning(const std::string& message) {
    std::clog << BOLD_YELLOW << "[WARNING] " << RESET << YELLOW << message << RESET << std::endl;
}

/**
 * @brief Displays a dynamic status bar on the current line in std::clog.
 * @param progress A value between 0.0 and 1.0 indicating completion.
 * @param width The total character width of the bar itself.
 */
inline void logStatusBar(double progress, int width = 25) {
    if (progress < 0.0)
        progress = 0.0;
    if (progress > 1.0)
        progress = 1.0;

    int bar_fill = static_cast<int>(progress * width);
    int percentage = static_cast<int>(progress * 100.0);

    // \r (carriage return) move o cursor para o início da linha
    std::clog << GREEN << "\rProgress: [" << RESET;
    for (int i = 0; i < width; ++i) {
        if (i < bar_fill)
            std::clog << "=";
        else
            std::clog << " ";
    }
    std::clog << GREEN << "] " << percentage << "%" << RESET << std::flush;

    if (progress >= 1.0) {
        std::clog << '\n' << std::endl;
    }
}

} // namespace Style
} // namespace Prism

#endif // PRISM_STYLE_HPP_
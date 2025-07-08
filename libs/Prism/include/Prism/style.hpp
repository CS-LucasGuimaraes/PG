#ifndef PRISM_STYLE_HPP_
#define PRISM_STYLE_HPP_

#include "prism_export.h"
#include <string>

namespace Prism {

/**
 * @namespace Style
 * @brief Provides constants for colored console output.
 *
 * This namespace centralizes ANSI escape codes for styling text in the terminal,
 * ensuring a consistent look and feel for all engine logs.
 */
namespace PRISM_EXPORT Style {

    // --- Basic Colors ---
    const std::string RESET      = "\033[0m";
    const std::string YELLOW     = "\033[0;33m";
    const std::string GREEN      = "\033[0;32m";
    const std::string CYAN       = "\033[0;36m";
    const std::string GRAY       = "\033[0;90m";
    const std::string RED        = "\033[0;31m";

    // --- Bold Variants ---
    const std::string BOLD_CYAN  = "\033[1;36m";
    const std::string BOLD_GREEN = "\033[1;32m";
    const std::string BOLD_RED   = "\033[1;31m";

} // namespace Style
} // namespace Prism

#endif // PRISM_STYLE_HPP_
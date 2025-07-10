#include "Prism/core/style.hpp"

#include <iostream>

namespace Prism {

/**
 * @struct PrismInitializer
 * @brief A helper class whose sole purpose is to run code in its
 * constructor, before the program's 'main'.
 */
struct PrismInitializer {
    PrismInitializer() {
        // This code will run once when the library is loaded.
        std::clog << Style::BOLD_CYAN << "\n[PRISM RENDER ENGINE]\n" << Style::RESET << std::endl;
    }
};

static PrismInitializer initializer;

} // namespace Prism
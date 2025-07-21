/**
 * @file init.cpp
 * @brief Prism Render Engine
 *
 * @copyright Copyright (c) 2025 src-lua
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "Prism/core/style.hpp"

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

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
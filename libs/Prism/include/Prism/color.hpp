#ifndef PRISM_COLOR_HPP_
#define PRISM_COLOR_HPP_

#include "prism_export.h"

namespace Prism {
    class PRISM_EXPORT Color {
        public:
            Color();
            Color(double red, double green, double blue);
            Color(int red, int green, int blue);
            Color(const Color& other);
            double r, g, b;
    };

} // namespace Prism
#endif // PRISM_COLOR_HPP_
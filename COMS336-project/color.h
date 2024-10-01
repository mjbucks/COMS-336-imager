#ifndef COLOR_H
#define COLOR_H

#include "interval.h"
#include "vec3.h"
#include "commons.h"

using color = vec3;

inline double lin_to_gam(double linear_component) {
    if (linear_component > 0)
        return std::sqrt(linear_component);
    
    return 0;
}

void write_color(std::ostream& out, const color& pixel_color) {
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    r = lin_to_gam(r);
    g = lin_to_gam(g);
    b = lin_to_gam(b);

    static const interval intensity(0.000, 0.999);

    int rbyte = int(256 * intensity.clamp(r));
    int gbyte = int(256 * intensity.clamp(g));
    int bbyte = int(256 * intensity.clamp(b));

    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}

#endif
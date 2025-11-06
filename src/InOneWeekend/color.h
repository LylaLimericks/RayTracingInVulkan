#ifndef COLOR_H
#define COLOR_H

#include "interval.h"
#include "vec3.h"

#include <iostream>

using color = vec3;

void writeColor(std::ostream& out, const color& pixelColor) {
    auto r = pixelColor.x();
    auto g = pixelColor.y();
    auto b = pixelColor.z();

    static const interval intensity(0.000, 0.999);

    int rbyte = int(256 * intensity.clamp(r));
    int gbyte = int(256 * intensity.clamp(g));
    int bbyte = int(256 * intensity.clamp(b));

    out << rbyte << ' ' << gbyte << ' ' << bbyte << "\n";
}

#endif
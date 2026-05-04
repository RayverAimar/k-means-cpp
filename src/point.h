#pragma once

#include <cmath>
#include <iostream>
#include <string>

struct Point {
    float f0, f1, f2, f3;
    std::string label;
    int cluster;

    Point() : f0(0), f1(0), f2(0), f3(0), cluster(-1) {}

    Point(float a, float b, float c, float d, std::string lbl = "") :
        f0(a), f1(b), f2(c), f3(d), label(lbl), cluster(-1) {}

    double distance(const Point& p) const {
        double d0 = p.f0 - f0, d1 = p.f1 - f1;
        double d2 = p.f2 - f2, d3 = p.f3 - f3;
        return std::sqrt(d0*d0 + d1*d1 + d2*d2 + d3*d3);
    }

    void print() const {
        std::cout << "(" << f0 << ", " << f1 << ") ("
                  << f2 << ", " << f3 << ") " << label << "\n";
    }
};

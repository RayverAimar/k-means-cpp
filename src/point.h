#pragma once

#include <cmath>
#include <iostream>
#include <string>
#include <vector>

struct Point {
    std::vector<float> features;
    std::string label;
    int cluster;

    Point() : cluster(-1) {}

    Point(std::vector<float> f, std::string lbl = "")
        : features(std::move(f)), label(lbl), cluster(-1) {}

    double distance(const Point& p) const {
        double sum = 0.0;
        for (size_t i = 0; i < features.size(); i++) {
            double d = p.features[i] - features[i];
            sum += d * d;
        }
        return std::sqrt(sum);
    }

    void print() const {
        std::cout << "[";
        for (size_t i = 0; i < features.size(); i++) {
            if (i) std::cout << ", ";
            std::cout << features[i];
        }
        std::cout << "] " << label << "\n";
    }
};

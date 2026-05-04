#pragma once

#include "point.h"
#include <vector>

static constexpr double KMEANS_TOL = 1e-6;

struct KMeansResult {
    std::vector<Point> centroids;
    int iterations;
    bool converged;
};

KMeansResult kmeans(std::vector<Point>& points, int k = 3, int max_iters = 300);

#include "kmeans.h"

#include <limits>
#include <vector>

KMeansResult kmeans(std::vector<Point>& points, int k, int max_iters) {
    std::vector<Point> centroids(points.begin(), points.begin() + k);
    for (auto& c : centroids) c.label = "";

    int iter = 0;
    bool converged = false;

    for (; iter < max_iters; iter++) {
        // Assignment: each point joins its nearest centroid
        for (auto& p : points) {
            double min_dist = std::numeric_limits<double>::max();
            for (int j = 0; j < k; j++) {
                double d = p.distance(centroids[j]);
                if (d < min_dist) {
                    min_dist = d;
                    p.cluster = j;
                }
            }
        }

        // Update: move centroid to mean of assigned points
        std::vector<Point> next(k);
        std::vector<int> counts(k, 0);
        for (const auto& p : points) {
            next[p.cluster].f0 += p.f0;
            next[p.cluster].f1 += p.f1;
            next[p.cluster].f2 += p.f2;
            next[p.cluster].f3 += p.f3;
            counts[p.cluster]++;
        }
        for (int j = 0; j < k; j++) {
            if (counts[j] > 0) {
                next[j].f0 /= counts[j];
                next[j].f1 /= counts[j];
                next[j].f2 /= counts[j];
                next[j].f3 /= counts[j];
            }
        }

        // Convergence: stop when no centroid moves more than TOL
        converged = true;
        for (int j = 0; j < k; j++) {
            if (centroids[j].distance(next[j]) > KMEANS_TOL) {
                converged = false;
                break;
            }
        }
        centroids = next;
        if (converged) break;
    }

    return {centroids, iter, converged};
}

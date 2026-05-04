#include "kmeans.h"

#include <limits>
#include <vector>

KMeansResult kmeans(std::vector<Point>& points, int k, int max_iters) {
    int dim = (int)points[0].features.size();

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
        for (auto& c : next) c.features.assign(dim, 0.0f);
        std::vector<int> counts(k, 0);

        for (const auto& p : points) {
            for (int d = 0; d < dim; d++)
                next[p.cluster].features[d] += p.features[d];
            counts[p.cluster]++;
        }
        for (int j = 0; j < k; j++) {
            if (counts[j] > 0)
                for (int d = 0; d < dim; d++)
                    next[j].features[d] /= counts[j];
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

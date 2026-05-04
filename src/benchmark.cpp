#include "kmeans.h"

#include <chrono>
#include <iomanip>
#include <iostream>
#include <random>
#include <vector>

// Generates N points drawn from K isotropic Gaussian clusters in 4D space.
std::vector<Point> generate_points(int n, int k, unsigned seed = 42) {
    std::mt19937 rng(seed);
    std::uniform_real_distribution<float> center_dist(0.0f, 20.0f);
    std::normal_distribution<float> noise(0.0f, 1.0f);

    // Pick K random cluster centers
    std::vector<std::array<float, 4>> centers(k);
    for (auto& c : centers)
        for (auto& v : c) v = center_dist(rng);

    std::vector<Point> pts;
    pts.reserve(n);
    std::uniform_int_distribution<int> which(0, k - 1);
    for (int i = 0; i < n; i++) {
        auto& c = centers[which(rng)];
        pts.emplace_back(c[0] + noise(rng), c[1] + noise(rng),
                         c[2] + noise(rng), c[3] + noise(rng));
    }
    return pts;
}

int main() {
    const int MAX_ITERS = 100;

    std::vector<int> sizes  = {10'000, 100'000, 1'000'000};
    std::vector<int> ks     = {3, 10};

    std::cout << std::left
              << std::setw(12) << "N"
              << std::setw(6)  << "K"
              << std::setw(10) << "Iters"
              << std::setw(10) << "Converged"
              << std::setw(14) << "Time (ms)"
              << "\n"
              << std::string(52, '-') << "\n";

    for (int k : ks) {
        for (int n : sizes) {
            auto pts = generate_points(n, k);

            auto t0 = std::chrono::high_resolution_clock::now();
            auto [centroids, iters, converged] = kmeans(pts, k, MAX_ITERS);
            auto t1 = std::chrono::high_resolution_clock::now();

            double ms = std::chrono::duration<double, std::milli>(t1 - t0).count();

            std::cout << std::left
                      << std::setw(12) << n
                      << std::setw(6)  << k
                      << std::setw(10) << iters
                      << std::setw(10) << (converged ? "yes" : "no")
                      << std::fixed << std::setprecision(2)
                      << std::setw(14) << ms
                      << "\n";
        }
        std::cout << "\n";
    }

    return 0;
}

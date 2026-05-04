#include "kmeans.h"

#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#define IRIS_PATH "dataset/iris.data"
#define DATASET_LENGTH 150

std::vector<Point> read_iris(const std::string& path) {
    std::vector<Point> points;
    std::ifstream file(path);
    std::string line;
    int idx = 0;
    while (std::getline(file, line) && idx++ < DATASET_LENGTH) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string a, b, c, d, cls;
        std::getline(ss, a, ',');
        std::getline(ss, b, ',');
        std::getline(ss, c, ',');
        std::getline(ss, d, ',');
        std::getline(ss, cls, ',');
        points.emplace_back(std::stof(a), std::stof(b), std::stof(c), std::stof(d), cls);
    }
    return points;
}

int main() {
    auto dataset = read_iris(IRIS_PATH);

    auto t0 = std::chrono::high_resolution_clock::now();
    auto [centroids, iters, converged] = kmeans(dataset, 3);
    auto t1 = std::chrono::high_resolution_clock::now();

    for (int j = 0; j < (int)centroids.size(); j++) {
        std::cout << "\nCLUSTER [" << j << "]\n\n";
        for (const auto& p : dataset) {
            if (p.cluster == j) {
                std::cout << j << " -> ";
                p.print();
            }
        }
    }

    std::cout << "\n";
    for (int i = 0; i < (int)centroids.size(); i++) {
        std::cout << "CENTROID [" << i << "]: ";
        centroids[i].print();
    }

    auto ms = std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count();
    std::cout << "\nConverged: " << (converged ? "yes" : "no (max_iters reached)")
              << "  |  Iterations: " << iters
              << "  |  Time: " << ms << " us\n";

    return 0;
}

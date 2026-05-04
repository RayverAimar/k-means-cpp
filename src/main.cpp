#include "kmeans.h"

#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

// Generic CSV reader. Parses any delimited file: feature_cols specifies which
// column indices to use as features; label_col is the class label (-1 = none).
std::vector<Point> read_csv(const std::string& path,
                             const std::vector<int>& feature_cols,
                             int label_col = -1) {
    std::vector<Point> points;
    std::ifstream file(path);
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::vector<std::string> fields;
        std::stringstream ss(line);
        std::string tok;
        while (std::getline(ss, tok, ','))
            fields.push_back(tok);

        std::vector<float> f;
        f.reserve(feature_cols.size());
        for (int col : feature_cols)
            f.push_back(std::stof(fields[col]));

        std::string lbl = (label_col >= 0) ? fields[label_col] : "";
        points.emplace_back(f, lbl);
    }
    return points;
}

void export_results(const std::vector<Point>& points,
                    const std::vector<Point>& centroids,
                    const std::string& pts_path,
                    const std::string& cen_path) {
    int dim = (int)points[0].features.size();

    std::ofstream pts_out(pts_path);
    for (int i = 0; i < dim; i++) pts_out << "f" << i << ",";
    pts_out << "cluster,label\n";
    for (const auto& p : points) {
        for (auto v : p.features) pts_out << v << ",";
        pts_out << p.cluster << "," << p.label << "\n";
    }

    std::ofstream cen_out(cen_path);
    cen_out << "cluster";
    for (int i = 0; i < dim; i++) cen_out << ",f" << i;
    cen_out << "\n";
    for (int i = 0; i < (int)centroids.size(); i++) {
        cen_out << i;
        for (auto v : centroids[i].features) cen_out << "," << v;
        cen_out << "\n";
    }
}

void run(const std::string& name, std::vector<Point>& dataset, int k,
         const std::string& pts_path, const std::string& cen_path) {
    std::cout << "\n=== " << name << " ===\n\n";

    auto t0 = std::chrono::high_resolution_clock::now();
    auto [centroids, iters, converged] = kmeans(dataset, k);
    auto t1 = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < (int)centroids.size(); i++) {
        std::cout << "CENTROID [" << i << "]: ";
        centroids[i].print();
    }

    auto us = std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count();
    std::cout << "\nConverged: " << (converged ? "yes" : "no")
              << "  |  Iterations: " << iters
              << "  |  Time: " << us << " us\n";

    export_results(dataset, centroids, pts_path, cen_path);
    std::cout << "Exported " << pts_path << " and " << cen_path << "\n";
}

int main() {
    // Iris: 4 features (cols 0-3), class label at col 4
    auto iris = read_csv("dataset/iris.data", {0, 1, 2, 3}, 4);
    run("Iris (150 samples, 4 features, K=3)", iris, 3,
        "iris_results.csv", "iris_centroids.csv");

    // Wine: 13 features (cols 1-13), class label at col 0
    auto wine = read_csv("dataset/wine.data", {1,2,3,4,5,6,7,8,9,10,11,12,13}, 0);
    run("Wine (178 samples, 13 features, K=3)", wine, 3,
        "wine_results.csv", "wine_centroids.csv");

    std::cout << "\nRun `python plot.py <results.csv> <centroids.csv>` to visualize.\n";
    return 0;
}

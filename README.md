# KMeans (C++)

A from-scratch K-Means clustering implementation in C++17 with no external dependencies. Ships with two executables: a demo that clusters the [Iris dataset](https://archive.ics.uci.edu/ml/machine-learning-databases/iris/) and a benchmark that measures wall-clock time on synthetic data up to 1 million points.

## Quickstart

```bash
git clone https://github.com/RayverAimar/k-means-cpp.git
cd k-means-cpp
make          # builds ./kmeans and ./benchmark
./kmeans      # Iris demo
./benchmark   # timing table
```

Requires a C++17-capable compiler (`g++` or `clang++`). No external libraries needed.

## Demo

`./kmeans` clusters 150 Iris samples into 3 groups and reports convergence stats:

```
CLUSTER [0]
0 -> (7, 3.2) (4.7, 1.4) Iris-versicolor
...

CENTROID [0]: (6.85384, 3.07692) (5.71538, 2.05385)
CENTROID [1]: (5.88361, 2.74098) (4.38853, 1.43443)
CENTROID [2]: (5.006, 3.418) (1.464, 0.244)

Converged: yes  |  Iterations: 11  |  Time: 54 us
```

## Benchmark

`./benchmark` generates synthetic 4D Gaussian clusters and measures wall-clock time (Apple M-series, `-O2`):

```
N           K     Iters     Converged Time (ms)
----------------------------------------------------
10000       3     21        yes       5.84
100000      3     100       no        305.21
1000000     3     100       no        2346.25

10000       10    22        yes       16.39
100000      10    100       no        710.22
1000000     10    100       no        7108.41
```

The naive initialization (first K points as centroids) struggles to converge on large random datasets within 100 iterations — at this scale, k-means++ initialization would converge faster and more reliably.

## How it works

1. **Initialize** — use the first K points in the dataset as initial centroids
2. **Assign** — assign each point to the nearest centroid (Euclidean distance over all 4 features)
3. **Update** — move each centroid to the mean of its assigned points
4. **Repeat** until convergence (`tol=1e-6`) or `max_iters`

Time complexity per iteration: `O(N × K)`. Total: `O(N × K × iters)`.

## File structure

```
src/
  point.h        — Point struct (4 features, Euclidean distance)
  kmeans.h       — KMeansResult struct + function declaration
  kmeans.cpp     — algorithm implementation
  main.cpp       — Iris dataset demo with timing
  benchmark.cpp  — synthetic data generator + timing table
dataset/
  iris.data      — UCI Iris dataset (150 samples)
Makefile
```

## Parameters

| Parameter   | Default | Description                               |
|-------------|---------|-------------------------------------------|
| `k`         | `3`     | Number of clusters                        |
| `max_iters` | `300`   | Maximum iterations (benchmark uses `100`) |
| `KMEANS_TOL`| `1e-6`  | Convergence threshold (centroid movement) |

Modify these in `src/kmeans.h` and `src/benchmark.cpp`.

## Note

Educational implementation. For production use, prefer well-tested libraries like [dlib](http://dlib.net/) or [mlpack](https://www.mlpack.org/) which offer k-means++ initialization, BLAS-accelerated distance computations, and parallelism.

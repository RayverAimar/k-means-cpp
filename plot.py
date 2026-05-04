"""
Usage:
  python plot.py <results.csv> <centroids.csv> [out.png] [xi] [yi] [xj] [yj]

Defaults: xi=0 yi=1 xj=2 yj=3 (first four features, two views side by side).
All feature columns must start with 'f' followed by an integer index.

Examples:
  python plot.py iris_results.csv iris_centroids.csv clusters_iris.png
  python plot.py wine_results.csv wine_centroids.csv clusters_wine.png 0 6 9 12
"""

import csv
import sys
import matplotlib.pyplot as plt
import matplotlib.patches as mpatches

COLORS = ["#e74c3c", "#2ecc71", "#3498db", "#f39c12", "#9b59b6"]

def load_csv(path):
    with open(path) as f:
        return list(csv.DictReader(f))

def feature_cols(rows):
    return sorted([k for k in rows[0] if k.startswith("f") and k[1:].isdigit()],
                  key=lambda k: int(k[1:]))

def scatter(ax, points, centroids, xk, yk, clusters):
    for c in clusters:
        xs = [float(p[xk]) for p in points if int(p["cluster"]) == c]
        ys = [float(p[yk]) for p in points if int(p["cluster"]) == c]
        ax.scatter(xs, ys, color=COLORS[c % len(COLORS)], alpha=0.6, s=40)
    for row in centroids:
        ax.scatter(float(row[xk]), float(row[yk]),
                   color="black", marker="X", s=180, zorder=5)
    ax.set_xlabel(xk)
    ax.set_ylabel(yk)

args     = sys.argv[1:]
pts_path = args[0] if len(args) > 0 else "iris_results.csv"
cen_path = args[1] if len(args) > 1 else "iris_centroids.csv"
out_path = args[2] if len(args) > 2 else pts_path.replace("_results.csv", "_clusters.png")

points    = load_csv(pts_path)
centroids = load_csv(cen_path)
clusters  = sorted(set(int(p["cluster"]) for p in points))
fcols     = feature_cols(points)

xi = f"f{args[3]}" if len(args) > 3 else fcols[0]
yi = f"f{args[4]}" if len(args) > 4 else fcols[1]
xj = f"f{args[5]}" if len(args) > 5 else fcols[min(2, len(fcols)-1)]
yj = f"f{args[6]}" if len(args) > 6 else fcols[min(3, len(fcols)-1)]

dataset_name = pts_path.replace("_results.csv", "").replace(".csv", "").capitalize()

fig, axes = plt.subplots(1, 2, figsize=(12, 5))
fig.suptitle(f"K-Means Clustering — {dataset_name} Dataset", fontsize=14, fontweight="bold")

scatter(axes[0], points, centroids, xi, yi, clusters)
scatter(axes[1], points, centroids, xj, yj, clusters)

legend = [mpatches.Patch(color=COLORS[c % len(COLORS)], label=f"Cluster {c}") for c in clusters]
legend.append(plt.Line2D([0], [0], marker="X", color="black",
                          linestyle="None", markersize=10, label="Centroid"))
axes[1].legend(handles=legend, loc="best")

plt.tight_layout()
plt.savefig(out_path, dpi=150)
print(f"Saved {out_path}")

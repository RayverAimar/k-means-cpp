import csv
import matplotlib.pyplot as plt
import matplotlib.patches as mpatches

COLORS = ["#e74c3c", "#2ecc71", "#3498db", "#f39c12", "#9b59b6"]
MARKERS_CENTROID = "X"

def load_csv(path):
    with open(path) as f:
        reader = csv.DictReader(f)
        return list(reader)

points = load_csv("results.csv")
centroids = load_csv("centroids.csv")

clusters = sorted(set(int(p["cluster"]) for p in points))

fig, axes = plt.subplots(1, 2, figsize=(12, 5))
fig.suptitle("K-Means Clustering — Iris Dataset", fontsize=14, fontweight="bold")

views = [
    ("sepal_length", "sepal_width",  "Sepal length (cm)", "Sepal width (cm)"),
    ("petal_length", "petal_width",  "Petal length (cm)", "Petal width (cm)"),
]

for ax, (xk, yk, xlabel, ylabel) in zip(axes, views):
    for c in clusters:
        xs = [float(p[xk]) for p in points if int(p["cluster"]) == c]
        ys = [float(p[yk]) for p in points if int(p["cluster"]) == c]
        ax.scatter(xs, ys, color=COLORS[c % len(COLORS)], alpha=0.6, s=40)

    for row in centroids:
        ax.scatter(float(row[xk]), float(row[yk]),
                   color="black", marker=MARKERS_CENTROID, s=180, zorder=5)

    ax.set_xlabel(xlabel)
    ax.set_ylabel(ylabel)
    ax.set_title(f"{xlabel.split()[0]} space")

legend = [mpatches.Patch(color=COLORS[c % len(COLORS)], label=f"Cluster {c}") for c in clusters]
legend.append(plt.Line2D([0], [0], marker=MARKERS_CENTROID, color="black",
                          linestyle="None", markersize=10, label="Centroid"))
axes[1].legend(handles=legend, loc="upper left")

plt.tight_layout()
plt.savefig("clusters.png", dpi=150)
print("Saved clusters.png")

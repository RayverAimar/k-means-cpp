import csv
import matplotlib.pyplot as plt
from collections import defaultdict

def load_csv(path):
    with open(path) as f:
        return list(csv.DictReader(f))

rows = load_csv("benchmark.csv")

by_k = defaultdict(list)
for r in rows:
    by_k[int(r["k"])].append((int(r["n"]), float(r["time_ms"]), r["converged"] == "1"))

COLORS = ["#3498db", "#e74c3c", "#2ecc71", "#f39c12"]

fig, ax = plt.subplots(figsize=(9, 5))

for i, (k, entries) in enumerate(sorted(by_k.items())):
    entries.sort()
    ns     = [e[0] for e in entries]
    times  = [e[1] for e in entries]
    convs  = [e[2] for e in entries]
    color  = COLORS[i % len(COLORS)]

    ax.plot(ns, times, marker="o", color=color, label=f"K={k}", linewidth=2)

    for n, t, conv in zip(ns, times, convs):
        if not conv:
            ax.annotate("no conv.", (n, t), textcoords="offset points",
                        xytext=(6, 4), fontsize=7, color=color)

ax.set_xscale("log")
ax.set_xlabel("Number of points (N)", fontsize=11)
ax.set_ylabel("Wall-clock time (ms)", fontsize=11)
ax.set_title("K-Means scaling — synthetic 4D data (max_iters=100, -O2)", fontsize=12)
ax.legend()
ax.grid(True, which="both", linestyle="--", alpha=0.4)

plt.tight_layout()
plt.savefig("benchmark.png", dpi=150)
print("Saved benchmark.png")

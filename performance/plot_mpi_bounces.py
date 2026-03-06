import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import json
import seaborn as sns

with open("performance/measures/mpi_bounces.json", "r") as f:
    df = json.load(f)

fig, ax = plt.subplots(figsize=(15,10), layout="constrained")

N = len(df)

bounces = np.array([item["bounces"] for item in df])
y_sec = np.array([item["elapsed_time_sec"] for item in df])
#Instructions per cycle (high=good)
IPC = np.array([item["instructions"]/item["cpu_cycles"] for item in df])


ax.scatter(bounces, y_sec, s=75)
ax.plot(bounces, y_sec)

ax.set_xlabel("Bounces", size=15)
ax.set_ylabel("time (seconds)", size=15)
ax.set_title(f"Evolution of time execution by number of bounces per image", size=20)
ax.grid(color='black', linestyle='-', linewidth=0.1)
ax.set_xticks(np.linspace(bounces.min(), bounces.max(), 2*N-1))
ax.set_yticks(np.linspace(0, y_sec.max(), 10))

fig.savefig(f"performance/results/plot_mpi_bounces.png", dpi=300)
plt.close()


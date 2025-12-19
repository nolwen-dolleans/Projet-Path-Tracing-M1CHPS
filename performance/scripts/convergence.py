#!/usr/bin/env python3
import numpy as np
import pandas as pd
import seaborn as sns
from PIL import Image
import math
import matplotlib.pyplot as plt


sns.set_theme(style="whitegrid", context="talk")
N = 7

def load_image(path):
	img = Image.open(path).convert("RGB")
	return np.asarray(img, dtype=np.float32) / 255.0

def mse(img, ref):
	return abs(np.mean((img - ref)))
	
def smse(img, ref):
	return np.sqrt(mse(img, ref))

def compare_images(image_path, reference_path):
	img = load_image(image_path)
	ref = load_image(reference_path)

	return smse(img, ref)

	
arr_error = []
reference_path = "images/image_32bit4096.ppm"

for samples in range(0, N):
    img_index = str(2 ** samples)
    path = f"images/image_32bit{img_index}.ppm"

    error = compare_images(path, reference_path)
    arr_error.append(error)

arr_error = np.array(arr_error)
sampling = 2 ** np.arange(N)

data = pd.DataFrame(columns=["N", "Global Mean Error"])
data["Global Mean Error"] = arr_error
data["N"] = sampling

reference = pd.DataFrame(columns=["N", "1/N^(1/2)"])
reference["N"] = sampling
reference["1/N^(1/2)"] = 1/np.sqrt(1*sampling)

fig, ax = plt.subplots(figsize=(16, 8))

sns.lineplot(x="N", y="Global Mean Error",label="Global Mean Error", data=data, color='green')
sns.lineplot(x="N", y="1/N^(1/2)",label="1/N^(1/2)", data=reference, color='blue')


ax.set_xscale("log", base=2)
ax.set_yscale("log", base=2)


ax.set_title("Path Tracing Convergence", fontsize=20, pad=15)
ax.set_xlabel("Samples (N)", fontsize=16)
ax.set_ylabel("Global Mean Error (RMSE)", fontsize=16)


lines, labels = ax.get_legend_handles_labels()
ax.legend(fontsize=14)

ax.grid(True, which="both", linestyle="--", alpha=0.5)

path = "results/convergence_monte_carlo.png"

plt.tight_layout()
fig.savefig(path)



#!/usr/bin/env python3
import numpy as np
import pandas as pd
import seaborn as sns
from PIL import Image
import math
import matplotlib.pyplot as plt


sns.set_theme(style="whitegrid", context="talk")

index_images = [1, 2, 3, 4 ,5 , 6, 7, 8, 16, 30, 50]
N = len(index_images)

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
reference_path = "images/1000 rebonds.ppm"

for samples in index_images:
    path = f"images/{samples} rebonds.ppm"

    error = compare_images(path, reference_path)
    arr_error.append(error)

arr_error = np.array(arr_error)

data = pd.DataFrame(columns=["N", "Global Mean Error"])
data["Global Mean Error"] = arr_error
data["N"] = np.array(index_images)

reference = pd.DataFrame(columns=["N", "1/N^(1/2)"])
reference["N"] = np.array(index_images)
#reference["1/N^(1/2)"] = 1/np.sqrt(1*sampling)

fig, ax = plt.subplots(figsize=(16, 8))

sns.lineplot(x="N", y="Global Mean Error",label="Global Mean Error", data=data, color='green')
sns.scatterplot(x="N", y="Global Mean Error",label="Global Mean Error", data=data, color='blue')
#sns.lineplot(x="N", y="1/N^(1/2)",label="1/N^(1/2)", data=reference, color='blue')



ax.set_title("Path Tracing Convergence", fontsize=20, pad=15)
ax.set_xlabel("Bounces (D)", fontsize=16)
ax.set_ylabel("Global Mean Error (RMSE)", fontsize=16)


lines, labels = ax.get_legend_handles_labels()
ax.legend(fontsize=14)

ax.grid(True, which="both", linestyle="--", alpha=0.5)

path = "curbes/convergence_bounces.png"

plt.tight_layout()
fig.savefig(path)



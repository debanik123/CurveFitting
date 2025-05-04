import numpy as np
import matplotlib.pyplot as plt

# Load optimized parameters (a, b, c)
with open("/home/dj/robotics/CurveFitting/result/04_05_2025_21_33_55_parametersG2O.txt", "r") as f:
    line = f.readline()
    parts = line.strip().split(":")[1].strip().split()
    a, b, c = map(float, parts)

# Load the real data
x_data = []
y_data = []

with open("/home/dj/robotics/CurveFitting/result/04_05_2025_21_33_55_readDataG2O.txt", "r") as f:
    lines = f.readlines()

for line in lines:
    if "|" in line and line.count("|") == 4:
        parts = line.strip().split("|")
        try:
            x = float(parts[2].strip())
            y = float(parts[3].strip())
            x_data.append(x)
            y_data.append(y)
        except ValueError:
            continue  # skip headers or bad lines

# Convert to numpy
x_data = np.array(x_data)
y_data = np.array(y_data)

# Predict using estimated parameters
y_pred = np.exp(a * x_data ** 2 + b * x_data + c)

# Plotting
plt.figure(figsize=(10, 6))
plt.scatter(x_data, y_data, label="Noisy Data", color='blue', s=15)
plt.plot(x_data, y_pred, label=f"Fitted Curve: exp({a:.2f}x² + {b:.2f}x + {c:.2f})", color='red')
plt.title("Curve Fitting with G2O (C++) and Python Visualization")
plt.xlabel("x")
plt.ylabel("y")
plt.legend()
plt.grid(True)
plt.show()

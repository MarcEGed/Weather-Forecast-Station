import numpy as np
import pandas as pd
from sklearn.neural_network import MLPRegressor

CSV_FILE = r"C:\Users\User\Desktop\Engineering Projects\weatherForecastStation\MLStuff\temperature.csv"
WINDOW = 5  # number of previous temperatures to use


df = pd.read_csv(CSV_FILE)

temps = df["temperature"].values
hour_norm = df["hour_norm"].values  # 0-1
day_norm = df["day_norm"].values    # 0-1

X = []
y = []

for i in range(WINDOW, len(temps)):
    deltas = [temps[i-j] - temps[i-j-1] for j in reversed(range(WINDOW))]
    features = deltas + [hour_norm[i], day_norm[i]]
    
    X.append(features)
    y.append(temps[i])

X = np.array(X)
y = np.array(y)

print("Samples:", len(X))
print("Input shape:", X.shape)


delta_mean = X[:, :WINDOW].mean()
delta_std = X[:, :WINDOW].std()

X_norm = X.copy()
X_norm[:, :WINDOW] = (X[:, :WINDOW] - delta_mean) / delta_std

y_mean = y.mean()
y_std = y.std()
y_norm = (y - y_mean) / y_std


model = MLPRegressor(
    hidden_layer_sizes=(10,),
    activation="relu",
    solver="adam",
    max_iter=10000,
    verbose=True
)

model.fit(X_norm, y_norm)
print("\nTraining finished\n")


# TEST
pred_norm = model.predict(X_norm[:5])
pred_real = pred_norm * y_std + y_mean
y_real = y[:5]

print("Predicted temperatures (°C):", pred_real)
print("Actual temperatures (°C):   ", y_real)


# EXPORT WEIGHTS
W1 = model.coefs_[0]
W2 = model.coefs_[1]

B1 = model.intercepts_[0]
B2 = model.intercepts_[1]

input_size = X.shape[1]

print("\n===== EXPORT FOR ESP8266 =====\n")

# W1
print(f"float W1[{input_size}][6] = {{")
for row in W1:
    print(" {", ",".join(map(str,row)), "},")
print("};\n")

# B1
print("float B1[6] = {", ",".join(map(str,B1)), "};\n")

# W2
print("float W2[6] = {", ",".join(map(str,W2.flatten())), "};\n")

# B2
print("float B2 =", B2[0], ";\n")

# normalization constants
print("float delta_mean =", delta_mean, ";")
print("float delta_std =", delta_std, ";")
print("float y_mean =", y_mean, ";")
print("float y_std =", y_std, ";")
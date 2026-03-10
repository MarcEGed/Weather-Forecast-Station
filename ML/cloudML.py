import socket
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from datetime import datetime

from sklearn.ensemble import HistGradientBoostingRegressor
from sklearn.metrics import mean_absolute_error, mean_squared_error

df = pd.read_csv("MLStuff/beirut_weather.csv")

df["time"] = pd.to_datetime(df["time"])

df["hour"] = df["time"].dt.hour
df["day"] = df["time"].dt.dayofyear
df["hour_sin"] = np.sin(2*np.pi*df["hour"]/24)
df["hour_cos"] = np.cos(2*np.pi*df["hour"]/24)
df["day_sin"] = np.sin(2*np.pi*df["day"]/365)
df["day_cos"] = np.cos(2*np.pi*df["day"]/365)
df["temp_lag1"] = df["temp"].shift(1)
df["temp_lag24"] = df["temp"].shift(24)
df["temp_lag48"] = df["temp"].shift(48)
df["humidity_lag24"] = df["humidity"].shift(24)
df["cloud_lag24"] = df["cloud"].shift(24)
df["temp_mean_24"] = df["temp"].rolling(24).mean()
df["temp_std_24"] = df["temp"].rolling(24).std()

df["target"] = df["temp"].shift(-24)

df = df.dropna()

features = [
    "temp",
    "humidity",
    "cloud",
    "rain",
    "hour_sin",
    "hour_cos",
    "day_sin",
    "day_cos",
    "temp_lag1",
    "temp_lag24",
    "temp_lag48",
    "humidity_lag24",
    "cloud_lag24",
    "temp_mean_24",
    "temp_std_24"
]

X = df[features]
y = df["target"]

split_index = int(len(df) * 0.8)

X_train = X.iloc[:split_index]
X_test = X.iloc[split_index:]

y_train = y.iloc[:split_index]
y_test = y.iloc[split_index:]


model = HistGradientBoostingRegressor(
    max_iter=500,
    learning_rate=0.03,
    max_depth=6,
    random_state=42
)

model.fit(X_train, y_train)


pred = model.predict(X_test)

mae = mean_absolute_error(y_test, pred)
rmse = np.sqrt(mean_squared_error(y_test, pred))

print("MAE:", mae)
print("RMSE:", rmse)


def predict_from_current(temp, humidity, cloud=0, rain=0):

    now = datetime.now()

    hour = now.hour
    day = now.timetuple().tm_yday

    hour_sin = np.sin(2*np.pi*hour/24)
    hour_cos = np.cos(2*np.pi*hour/24)

    day_sin = np.sin(2*np.pi*day/365)
    day_cos = np.cos(2*np.pi*day/365)

    temp_lag1 = df["temp"].iloc[-1]
    temp_lag24 = df["temp"].iloc[-24]
    temp_lag48 = df["temp"].iloc[-48]

    humidity_lag24 = df["humidity"].iloc[-24]
    cloud_lag24 = df["cloud"].iloc[-24]

    temp_mean_24 = df["temp"].iloc[-24:].mean()
    temp_std_24 = df["temp"].iloc[-24:].std()

    input_data = pd.DataFrame([{
        "temp": temp,
        "humidity": humidity,
        "cloud": cloud,
        "rain": rain,
        "hour_sin": hour_sin,
        "hour_cos": hour_cos,
        "day_sin": day_sin,
        "day_cos": day_cos,
        "temp_lag1": temp_lag1,
        "temp_lag24": temp_lag24,
        "temp_lag48": temp_lag48,
        "humidity_lag24": humidity_lag24,
        "cloud_lag24": cloud_lag24,
        "temp_mean_24": temp_mean_24,
        "temp_std_24": temp_std_24
    }])

    prediction = model.predict(input_data)

    return prediction[0]


HOST = "0.0.0.0"  # listen on all interfaces
PORT = 6000

server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server.bind((HOST, PORT))
server.listen(1)
print(f"Server listening on {HOST}:{PORT}")

while True:
    conn, addr = server.accept()
    print(f"Connection from {addr}")

    try:
        data = conn.recv(1024).decode()
        if not data:
            conn.close()
            continue

        temp, humidity = map(float, data.strip().split(","))
        predicted_temp = predict_from_current(temp, humidity)
        conn.sendall(str(predicted_temp).encode())
        print(f"Received: {temp},{humidity} -> Sent prediction: {predicted_temp}")

    except Exception as e:
        print("Error:", e)
        conn.sendall(b"Error")

    conn.close()
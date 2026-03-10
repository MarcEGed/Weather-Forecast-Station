import requests
import pandas as pd
from datetime import datetime

LATITUDE = 33.89
LONGITUDE = 35.50
START_DATE = "2023-01-01"
END_DATE = "2024-01-01"
CSV_FILE = r"C:\Users\User\Desktop\Engineering Projects\weatherForecastStation\MLStuff\temperature.csv"


url = "https://archive-api.open-meteo.com/v1/archive"

params = {
    "latitude": LATITUDE,
    "longitude": LONGITUDE,
    "start_date": START_DATE,
    "end_date": END_DATE,
    "hourly": "temperature_2m"
}

response = requests.get(url, params=params)
data = response.json()

times = data["hourly"]["time"]
temps = data["hourly"]["temperature_2m"]


rows = []

for t_str, temp in zip(times, temps):
    dt = pd.to_datetime(t_str)
    hour_norm = dt.hour / 23       #0-1
    day_norm = dt.dayofyear / 365  #0-1
    rows.append({
        "time": t_str,
        "temperature": temp,
        "hour_norm": hour_norm,
        "day_norm": day_norm
    })

df = pd.DataFrame(rows)


df.to_csv(CSV_FILE, index=False)
print(f"Saved {len(df)} rows to {CSV_FILE}")
# ESP8266 Based Weather & Forecast Station

An ESP8266-based weather monitoring and prediction system that is extremely cheap to build.

## Features
- Sensor monitoring: temperature, humidity, light, and water/rain levels
- OLED display interface
- WiFi time synchronization and data transmission
- Next-hour prediction (on-edge): neural network on ESP8266 using last 6 hourly readings
- Next-day prediction (online): Python Histogram Gradient Boosting Regressor running on PC, ESP8266 sends current readings via WiFi and receives tomorrow’s forecast

#ifndef SENSORS_H
#define SENSORS_H
#include <DHT.h>

#define DHT_PIN     2    // GPIO5
#define ANALOG_PIN   A0   // Analog pin
#define PHOTO_PIN_MULT 14 //(D5)
#define WATER_PIN_MULT 15 //(D8)
#define WAKE_BTN 13 //(D7)

#define DHTTYPE DHT11
extern DHT dht;
extern int sensor_results[4];
extern volatile bool wake_Flag;

void IRAM_ATTR buttonISR();

void initialize_sensors();

void sample_DHT11(int conditions[]);
void sample_analogSensors(int conditions[]);

#endif
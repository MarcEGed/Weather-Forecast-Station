#include "sensors.h"

DHT dht(DHT_PIN, DHTTYPE);
int sensor_results[4];

volatile bool wake_Flag = false;

void IRAM_ATTR buttonISR(){
  wake_Flag = true;
}

void initialize_sensors(){
    pinMode(PHOTO_PIN_MULT, OUTPUT);
    pinMode(WATER_PIN_MULT, OUTPUT);
    pinMode(WAKE_BTN, INPUT_PULLUP);
    dht.begin();
}

void sample_DHT11(int conditions[]){
    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();
    conditions[0] = temperature;
    conditions[1] = humidity;
}

void analog_multiplexer(int pin, int conditions[]){
    digitalWrite(pin, 1);
    float value = analogRead(ANALOG_PIN);
    digitalWrite(pin, 0);

    int index = pin - 12;
    conditions[index] = value;
}

void sample_analogSensors(int conditions[]){
    analog_multiplexer(PHOTO_PIN_MULT, conditions);
    analog_multiplexer(WATER_PIN_MULT, conditions);
}
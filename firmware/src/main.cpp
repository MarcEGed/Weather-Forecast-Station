#include "../lib/tasks/tasks.h"

void setup() {
    Serial.begin(115200);

    initialize_sensors();
    initialize_screen();

    updateMode(); //check if online or offline

    if (!offlineMode) {
        draw_connecting_screen();
        initialize_wifi();
    }
    

    last_Interaction = millis();

    addTask(readSensors, 2000);       // every 2s
    addTask(updatePrediction, 20000); // every 20s
    addTask(updateDisplay, 100);      // every 0.1s
    addTask(updateHourlyTemp, 600000); // every 600s
}

void loop() {
    runScheduler();
}
#ifndef TASKS_H
#define TASKS_H

#include "sensors.h"
#include "screen.h"
#include "wifi.h"
#include "NN.h"
#include "save_temps.h"

extern unsigned long last_Interaction;

struct Task {
    void (*func)();          
    unsigned long interval;
    unsigned long last_Ran; 
};

void addTask(void (*f)(), unsigned long intervalMs);
void runScheduler();

void readSensors();
void updatePrediction();
void updateDisplay();
void updateHourlyTemp();

#endif
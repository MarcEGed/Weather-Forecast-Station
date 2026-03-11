#include "tasks.h"
#define MAX_TASKS 10

bool last_ButtonState = HIGH;
Task tasks[MAX_TASKS];
int num_Tasks = 0;   //number of tasks loaded

float temps[6];
float next_HourPred = 0;
float next_DayPred = 0;
int current_Mode = 0; //0=LIVE, 1=PREDICTION
unsigned long last_Interaction = 0;
int last_Hour = -1;

bool offlineMode = false;
const int OFFLINE_BTN = WAKE_BTN; // your pushbutton pin
const unsigned long OFFLINE_TIMEOUT = 10000; // 10 seconds

void addTask(void (*f)(), unsigned long intervalMs){
    tasks[num_Tasks].func = f;
    tasks[num_Tasks].interval = intervalMs;
    tasks[num_Tasks].last_Ran = 0;
    num_Tasks++;
}

void runScheduler(){
    unsigned long now = millis();
    for (int i = 0; i < num_Tasks; i++) {
        if (now - tasks[i].last_Ran >= tasks[i].interval) {
            tasks[i].func();
            tasks[i].last_Ran = now;
        }
    }
}

void updateMode(){
    Serial.println("Press the button in 10 seconds to enter offline mode...");
    unsigned long start = millis();
    int lastShown = -1;

    while (millis() - start < OFFLINE_TIMEOUT) {
        if (digitalRead(OFFLINE_BTN) == LOW) {
            offlineMode = true;
            Serial.println("Offline mode enabled!");
            break;
        }

        //Get remaining seconds
        int remaining = 10 - ((millis() - start) / 1000);

        //update if num changes
        if (remaining != lastShown && remaining >= 0) {
            lastShown = remaining;
            draw_choice_screen(remaining);
        }

        delay(20);
    }

    if (!offlineMode) {
        Serial.println("Online mode");
    }
}

void enter_light_sleep(){
    wake_Flag = false;

    Serial.println("Entering light sleep");
    WiFi.forceSleepBegin();
    delay(1);

    attachInterrupt(digitalPinToInterrupt(WAKE_BTN), buttonISR, FALLING);


    while (!wake_Flag) {
        delay(50);
    }

    detachInterrupt(digitalPinToInterrupt(WAKE_BTN));

    Serial.println("Waking up");
    WiFi.forceSleepWake();
    delay(1);
}

void readSensors(){
    sample_DHT11(sensor_results);
    sample_analogSensors(sensor_results);
}

void updatePrediction(){
    if (offlineMode) return;
    float norm_hour = normalize_hour(get_hour());
    float norm_day  = normalize_day(get_day());
    next_HourPred = predict_NextHour(temps, norm_hour, norm_day);
    next_DayPred = send_and_receive(String(sensor_results[0]), String(sensor_results[1]));
}

void updateDisplay(){
    bool current_ButtonState = digitalRead(WAKE_BTN);

    if (last_ButtonState == HIGH && current_ButtonState == LOW) {
        current_Mode = 1 - current_Mode;
        last_Interaction = millis();
    }

    last_ButtonState = current_ButtonState;

    if (millis() - last_Interaction > 360000) { //6 min inactivity
        draw_sleep_screen();
        enter_light_sleep();
        return;
    }

    if (current_Mode == 0)
        draw_live(sensor_results, get_time());
    else
        draw_predictions_screen(next_HourPred, next_DayPred, get_time(), get_hour());
}

void updateHourlyTemp(){
    int hour = get_hour();
    if (hour != last_Hour) {
        last_Hour = hour;
        float temp = sensor_results[0];
        update_Temps(temps, temp);
        Serial.println("Saved hourly temperature");
    }
}
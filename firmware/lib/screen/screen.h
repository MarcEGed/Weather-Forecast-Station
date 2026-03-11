#ifndef SCREEN_H
#define SCREEN_H

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
extern Adafruit_SSD1306 display;

void initialize_screen();
void draw_test(float conditions[]);
void clear_display();
void draw_choice_screen(int countdown);
void draw_live(int conditions[], String fetchedTime);
void draw_connecting_screen();
void draw_sleep_screen();
void draw_predictions_screen(float hourPred, float dayPred, String fetchedTime, int currentHour);

#endif
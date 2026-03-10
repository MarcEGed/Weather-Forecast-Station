#ifndef WIFI_H
#define WIFI_H
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>

extern const char* ssid;
extern const char* password;

void initialize_wifi();
float send_and_receive(String temp, String humidity);
int get_hour();
int get_day();
String get_time();

#endif
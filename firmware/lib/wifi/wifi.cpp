#include "wifi.h"

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 7200, 60000);

const char* ssid = "i'd like to think";
const char* password = "im smarter than to put ";
const char* serverIP = "my credentials on the internet"; 
const int serverPort = 6000;

WiFiClient client;

String currentTime;

void initialize_wifi(){
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi");

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi connected!");

    timeClient.begin();
}

float send_and_receive(String temp, String humidity){
    String payload = temp + "," + humidity;
    float predicted = NAN;

    if (client.connect(serverIP, serverPort)) {
        client.print(payload);
        Serial.println("Sent: " + payload);

        while (!client.available()) {
            delay(10);
        }

        String response = "";
        while (client.available()) {
            char c = client.read();
            response += c;
        }

        if (response.length() > 0) {
            predicted = response.toFloat();
            Serial.println("Received predicted temp: " + String(predicted));
        } else {
            Serial.println("No response from server");
        }

        client.stop();
    } else {
        Serial.println("Connection failed");
    }

    return predicted;
}

int get_hour(){
    return timeClient.getHours();
}

int get_day(){
    timeClient.update();
    time_t rawTime = timeClient.getEpochTime();
    struct tm *ptm = gmtime(&rawTime);
    return ptm->tm_yday + 1;  // 1–365
}

String get_time(){
    timeClient.update();
    if (timeClient.getMinutes() >= 10){
        currentTime = String(timeClient.getHours()) + ":" + String(timeClient.getMinutes());
    }else{
        currentTime = String(timeClient.getHours()) + ":" + "0"+ String(timeClient.getMinutes());
    }
    return currentTime;
}
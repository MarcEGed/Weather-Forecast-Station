#include "bitmaps.h"
#include "screen.h"

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void initialize_screen(){
    Serial.begin(115200);
    Wire.begin(4, 5);  // SDA=GPIO2, SCL=GPIO14

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED failed");
    while(true);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
}

void clear_display(){
  display.clearDisplay();
}

void draw_choice_screen(int countdown) {
  display.clearDisplay();

  display.drawLine(1, 12, 127, 12, 1);

  display.setTextColor(1);
  display.setTextWrap(false);
  display.setCursor(75, 2);
  display.print("boot mode");

  display.setCursor(2, 15);
  display.print("Press button to enter");

  display.setCursor(20, 27);
  display.print("offline mode...");

  display.setTextSize(1);
  display.setCursor(55, 40);
  display.print(countdown);

  display.display();
}

void draw_connecting_screen(){
  display.clearDisplay();
  display.drawBitmap(55, 21, image_wifi_3_bars_bits, 19, 16, 1);

  display.setTextColor(1);
  display.setTextWrap(false);
  display.setCursor(26, 39);
  display.print("Connecting...");

  display.display();
}

void draw_test(float conditions[]){
    display.clearDisplay();
    display.println(conditions[0]);
    display.println(conditions[1]);
    display.println(conditions[2]);
    display.println(conditions[3]);
    display.println(conditions[4]);
    display.println(conditions[5]);
    display.println(conditions[6]);


    display.display();
}


void draw_live(int conditions[], String fetchedTime){
  display.clearDisplay();
  display.drawLine(1, 12, 127, 12, 1);

  display.setTextColor(1);
  display.setTextWrap(false);
  display.setCursor(104, 2);
  display.print("Live");

  display.setCursor(3, 2);
  display.print(fetchedTime);

  display.drawBitmap(7, 24, image_weather_temperature_bits, 16, 16, 1);

  display.setCursor(9, 44);
  display.println(conditions[0]);

  display.drawBitmap(38, 24, image_weather_humidity_bits, 11, 16, 1);

  display.setCursor(41, 44);
  display.print(conditions[1]);

  display.drawBitmap(106, 23, image_weather_cloud_rain_bits, 17, 16, 1);

  display.drawBitmap(71, 23, image_cloud_1_bits, 17, 16, 1);

  display.setCursor(76, 44);
  display.print(conditions[2]);

  display.setCursor(111, 46);
  display.print(conditions[3]);

  display.setCursor(2, 56);
  display.print("Temp");

  display.setCursor(35, 56);
  display.print("Hum");

  display.setCursor(70, 56);
  display.print("Cld");

  display.setCursor(105, 56);
  display.print("Rain");

  display.display();
}

void draw_sleep_screen(){
  display.clearDisplay();
  display.drawLine(1, 12, 127, 12, 1);

  display.setTextColor(1);
  display.setTextWrap(false);
  display.setCursor(97, 2);
  display.print("Sleep");

  display.drawBitmap(49, 37, image_Layer_3_bits, 31, 27, 1);

  display.setCursor(17, 15);
  display.print("Press button");

  display.setCursor(65, 25);
  display.print("to wake...");

  display.display();
}

void draw_predictions_screen(float hourPred, float dayPred, String fetchedTime, int currentHour){
  int nextHour;
  if (currentHour == 23){
    nextHour = 0;
  }else{
    nextHour = currentHour + 1;
  }

  display.clearDisplay();
  display.drawLine(1, 12, 127, 12, 1);

  display.setTextColor(1);
  display.setTextWrap(false);
  display.setCursor(104, 2);
  display.print("Next");

  display.setCursor(8, 18);
  display.print("NN");

  display.setCursor(54, 18);
  display.print("@");

  display.setCursor(64, 18);
  display.print(nextHour);

  display.setCursor(23, 18);
  display.print("pred:");

  display.setCursor(93, 18);
  display.print(String(hourPred, 1));

  display.setCursor(121, 18);
  display.print("C");

  display.setCursor(8, 46);
  display.print("AI");

  display.setCursor(23, 46);
  display.print("pred:");

  display.setCursor(54, 46);
  display.print("@");

  display.setCursor(3, 3);
  display.print(fetchedTime);

  display.setCursor(64, 46);
  display.print("tmrw");

  display.setCursor(93, 46);
  display.print(String(dayPred, 1));

  display.setCursor(121, 46);
  display.print("C");

  display.setCursor(2, 37);
  display.print("Cloud");

  display.display();

}
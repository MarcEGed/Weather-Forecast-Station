#include "save_temps.h"

void load_Temps(float temps[6]){
    EEPROM.begin(EEPROM_SIZE);
    for (int i = 0; i < 6; i++){
        float val;
        EEPROM.get(i * sizeof(float), val);
        temps[i] = val;
    }
    EEPROM.end();
}

void save_Temps(float temps[6]){
    EEPROM.begin(EEPROM_SIZE);
    for (int i = 0; i < 6; i++){
        EEPROM.put(i * sizeof(float), temps[i]);
    }
    EEPROM.commit();
    EEPROM.end();
}

void update_Temps(float temps[6], float newTemp){
    //shift left, erase oldest
    for (int i = 0; i < 5; i++){
        temps[i] = temps[i+1];
    }
    temps[5] = newTemp;
    save_Temps(temps);
}
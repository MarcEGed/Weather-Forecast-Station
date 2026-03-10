#ifndef SAVE_TEMPS_H
#define SAVE_TEMPS_H

#include <EEPROM.h>

#define EEPROM_SIZE 512 //(4*6 needed, but this seems to be a standard)

void load_Temps(float temps[6]);

void save_Temps(float temps[6]);

void update_Temps(float temps[6], float newTemp);

#endif
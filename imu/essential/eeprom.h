#ifndef EEPROM_UTIL_H
#define EEPROM_UTIL_H

#include <EEPROM.h>
#include "util.h"

#define EEPROM_GYRO_A_OFFSET 0
#define EEPROM_GYRO_B_OFFSET 1
#define EEPROM_MAGNETO_OFFSET 2

void write_on_eeprom(int offset, vec3float *data)
{
    int index = offset * 3 * 4;
    EEPROM.put(index, *data);
}

void read_from_eeprom(int offset, vec3float &data)
{
    int index = offset * 3 * 4;
    EEPROM.get(index, data);
}

#endif

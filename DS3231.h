#ifndef DS3231_H
#define DS3231_H

#include "I2CDevice.h"
#include <cstdint>

class DS3231 : public EE513::I2CDevice {
public:
    DS3231(unsigned int bus, unsigned int device);
    void readTime();
    void printTime();
    void setTime(uint8_t seconds, uint8_t minutes, uint8_t hours, uint8_t dayOfWeek, uint8_t dayOfMonth, uint8_t month, uint8_t year);

private:
    uint8_t seconds, minutes, hours, dayOfWeek, dayOfMonth, month,year;
    uint8_t bcdToDec(uint8_t bcd);
    uint8_t decToBcd(uint8_t dec);
};

#endif

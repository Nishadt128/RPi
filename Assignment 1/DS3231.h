#ifndef DS3231_H
#define DS3231_H

#include "I2CDevice.h"    // Include the base I2CDevice class
#include <cstdint>      // Include fixed-width integer types
#include <ctime>        // Include time-related functions
#include <iostream>     // Include I/O stream for error reporting
#include <iomanip>      // Include I/O manipulators for formatting

//DS3231 class definition
class DS3231 : public EE513::I2CDevice {
public:
// Constructor: Initializes the DS3231 object with the I2C bus and device address
    DS3231(unsigned int bus, unsigned int device);
    //Method to read current time from DS3231 RTC    
    void readTime();
    //Method to print current time from DS3231 RTC
    void printTime();    //Method to print the current time from DS3231 RTC
    //Method to set current time from DS3231 RTC
    void setTime(uint8_t seconds, uint8_t minutes, uint8_t hours, uint8_t dayOfWeek, uint8_t dayOfMonth, uint8_t month, uint8_t year);
    //Method to set alarm 1 
    void setAlarm1(uint8_t seconds, uint8_t minutes, uint8_t hours);
    //Method to set Alarm 2
    void setAlarm2(uint8_t minutes, uint8_t hours);
    //Method to Print the temperature from DS3231 RTC
    void printTemperature();
    //Method to check alarm status and print if they have been triggered
    void checkAlarms();
    //Method to set the square wave output frequency 
    void setSquareWave(uint8_t frequency);
    //Method to enable or disable the interupt signal
    void setInterruptSignal(bool enable)

private:
    //Time components
    uint8_t seconds, minutes, hours, dayOfWeek, dayOfMonth, month,year;
    uint8_t bcdToDec(uint8_t bcd);    //BCD to decimal conversion
    uint8_t decToBcd(uint8_t dec);    //Decimal to BCD conversion
    float temperature;    //Temperature value in degree celsius
    time_t lastSetTime;   //Time when the RTC was last set
};

#endif

#include "DS3231.h"
#include <iostream>
#include <iomanip>

using namespace std;

DS3231::DS3231(unsigned int bus, unsigned int device) : EE513::I2CDevice(bus, device) {}

uint8_t DS3231::bcdToDec(uint8_t bcd) {
    return ((bcd >> 4) * 10 + (bcd & 0x0F));
}
uint8_t DS3231::decToBcd(uint8_t dec) {
    return ((dec / 10) << 4) | (dec % 10);
}
void DS3231::readTime() {
    unsigned char* buffer = this->readRegisters(7, 0x00);

    if (buffer == nullptr) {
        cerr << "Failed to read time from DS3231" << endl;
        return;
    }

    seconds = bcdToDec(buffer[0]);
    minutes = bcdToDec(buffer[1]);
    hours = bcdToDec(buffer[2] & 0x3F);
    dayOfWeek = bcdToDec(buffer[3]);
    dayOfMonth = bcdToDec(buffer[4]);
    month = bcdToDec(buffer[5] & 0x1F);
    year = bcdToDec(buffer[6]);
    
    delete[] buffer;
}

void DS3231::printTime() {
    readTime(); //Read time
    cout << "Date: " << setw(2) << setfill('0') << static_cast<int>(dayOfMonth) << "/"
         << setw(2) << setfill('0') << static_cast<int>(month) << "/"
         << setw(2) << setfill('0') << static_cast<int>(year) << "  "
         << "Time: " << setw(2) << setfill('0') << static_cast<int>(hours) << ":"
         << setw(2) << setfill('0') << static_cast<int>(minutes) << ":"
         << setw(2) << setfill('0') << static_cast<int>(seconds) << endl;
    
}
void DS3231::setTime(uint8_t seconds, uint8_t minutes, uint8_t hours, uint8_t dayOfWeek, uint8_t dayOfMonth, uint8_t month, uint8_t year) {
    unsigned char buffer[7];

    buffer[0] = decToBcd(seconds);
    buffer[1] = decToBcd(minutes);
    buffer[2] = decToBcd(hours);
    buffer[3] = decToBcd(dayOfWeek);
    buffer[4] = decToBcd(dayOfMonth);
    buffer[5] = decToBcd(month);
    buffer[6] = decToBcd(year);

    this->writeRegisters(0x00, buffer, 7);
}

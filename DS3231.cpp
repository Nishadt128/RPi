#include "DS3231.h"
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <ctime>

using namespace std;

DS3231::DS3231(unsigned int bus, unsigned int device) : EE513::I2CDevice(bus, device) {
    lastSetTime = time(nullptr);
}

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

    if(this->writeRegisters(0x00, buffer, 7) != 0){
        derr << "Failed to write time to DS3231" << endl:
        }
}
void DS3231::setAlarm1(uint8_t seconds, uint8_t minutes, uint8_t hours){
    unsigned char buffer[4];
    
    buffer[0]= decToBcd(seconds);
    buffer[1]= decToBcd(minutes);
    buffer[2]= decToBcd(hours);
    buffer[3]=0x80;
    
    this->writeRegisters(0x07, buffer, 4);

    unsigned char controlReg[2];
    this->readRegisters(2, 0x0E);
    controlReg[0] |=0x01;
    this->writeRegisters(0x0E,controlReg, 1);
}

void DS3231::setAlarm2(uint8_t minutes, uint8_t hours){
    unsigned char buffer[2];
    
    buffer[0]= decToBcd(minutes);
    buffer[1]= decToBcd(hours);
    buffer[2]= 0x80;
   
    this->writeRegisters(0x0B, buffer, 3);
    
    unsigned char controlReg[2];
    this->readRegisters(2, 0x0E);
    controlReg[0] |=0x02;
    this->writeRegisters(0x0E,controlReg, 1);
}

void DS3231::printTemperature(){
    unsigned char* buffer = this->readRegisters(2, 0x11);

    if (buffer == nullptr) {
        cerr << "Failed to read temperature from DS3231" << endl;
        return;
        }
    int8_t temp_msb = buffer[0];
    uint8_t temp_lsb = buffer[1]>>6;
    
    float temperature = temp_msb + (temp_lsb*0.25);
    cout << "Temperature: " << temperature << "C" << endl;   

    delete[] buffer;
}

void DS3231::checkAlarms(){
    unsigned char* buffer = this->readRegisters(1,0x0F);
    
    if(buffer == nullptr){
        cerr << "Failed to read alarm status from DS3231" << endl;
        return;
    }
    uint8_t status = buffer[0];
    bool alarm1Triggered = status & 0x01;
    bool alarm2Triggered = status & 0x02;
    
    if(alarm1Triggered){
        cout << "Alarm 1 triggered" << endl;
        status &= ~0x01;
    }
    if(alarm2Triggered){
        cout << "Alarm 2 triggered" << endl;
        status &= ~0x01;
    }
    
    delete[] buffer;
}

void DS3231::setSquareWave(uint8_t frequency){
    unsigned char controlReg = 0x00;
    switch(frequency){
        case 1: controlReg = 0x10; break;
        case 2: controlReg = 0x11; break;
        case 4: controlReg = 0x12; break;
        case 8: controlReg = 0x13; break;
        default: controlReg = 0x00; break;
    }
    this->writeRegisters(0x0E, &controlReg, 1);
}

void DS3231::setInterruptSignal(bool enable){
    unsigned char controlReg[2];
    this->readRegisters(2, 0x0E);
    
    if(enable){
        controlReg[0] |= 0x80;
    }else{
        controlReg[0] &= ~0x80;
    }
    this->writeRegisters(0x0E, controlReg ,1);
}

void DS3231::printElapsedTime(){
    time_t currentTime = time(nullptr);
    double elapsed = difftime(currentTime, lastSetTime);
    
    int hours = static_cast<int>(elapsed)/3600;
    int minutes = (static_cast<int>(elapsed)%3600)/60;
    int seconds = static_cast<int>(elapsed)%60;
    
    cout << "Time elapsed since last set:"
         <<setw(2)<<setfill('0')<<hours<<":"
         <<setw(2)<<setfill('0')<<minutes<<":"
         <<setw(2)<<setfill('0')<<seconds<<endl;
}



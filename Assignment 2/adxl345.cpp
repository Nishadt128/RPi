#include <iostream>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <unistd.h>
#include "adxl345.h"

#define ADXL345_ADDR 0x53 // ADXL345 device address
#define ADXL345_POWER_CTL 0x2D
#define ADXL345_DATAX0 0x32

ADXL345::ADXL345() {
    fd = wiringPiI2CSetup(ADXL345_ADDR);
    if (fd == -1) {
        std::cerr << "Failed to initialize I2C communication." << std::endl;
        exit(-1);
    }
    // Set the device to measure mode
    wiringPiI2CWriteReg8(fd, ADXL345_POWER_CTL, 0x08);
}

void ADXL345::readXYZ(int &x, int &y, int &z) {
    x = wiringPiI2CReadReg16(fd, ADXL345_DATAX0);
    y = wiringPiI2CReadReg16(fd, ADXL345_DATAX0 + 2);
    z = wiringPiI2CReadReg16(fd, ADXL345_DATAX0 + 4);
}

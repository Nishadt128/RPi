#ifndef ADXL345_H
#define ADXL345_H

class ADXL345 {
public:
    ADXL345();
    void readXYZ(int &x, int &y, int &z);

private:
    int fd;
};

#endif // ADXL345_H

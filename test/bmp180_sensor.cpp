#include "bmp180.hpp"

int main(){
    BMP180 bmp180;
    bmp180.readCalibrationCoef();
    bmp180.prinfCalbrationCoef();
    return 0;
}
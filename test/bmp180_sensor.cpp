#include "bmp180.hpp"

int main(){
    BMP180 bmp180;
    bmp180.readCalibrationCoef();
    bmp180.prinfCalbrationCoef();
    std::cout << "Temperature = " << bmp180.readTemperature() <<" *C "<< std::endl;
    // std::cout << "Pressure = " << bmp180.readPressure() << " Pa" <<std::endl;
    // pressure of 1013.25 millibar = 101325 Pascal
    // std::cout << "Altitude = " << bmp180.readAltitude() << " meters" << std::endl;
    // std::cout << "Pressure at sealevel (calculated) = " 
                    // << bmp180.readSealevelPressure() << " Pa" << std::endl;

    // std::cout << "Real altitude = " << bmp180.readAltitude(101500) << " meters" <<std::endl;
 
    return 0;
}
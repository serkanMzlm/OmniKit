#include "vl53l5cx.hpp"
#include "vl53l5cx_type.hpp"
#include <iomanip>
extern "C"{
#include <errno.h>
#include <unistd.h>
}

VL53L5CX::VL53L5CX(){
    if(init() < 0){
        exit(-1);
    }
}

VL53L5CX::~VL53L5CX(){

}

int VL53L5CX::init(){
    if(vl53l5cx_comms_init(&dev.platform)){
        reportError(errno, "Device's initialization failed");
        return -1;
    }
    status = vl53l5cx_is_alive(&dev, &isAlive);
    if(!isAlive || status){
        reportError(errno);
        return -1;
    }
    status = vl53l5cx_init(&dev);
    usleep(DELAY);
    status = vl53l5cx_set_ranging_frequency_hz(&dev, 15);
    usleep(DELAY);
    vl53l5cx_set_resolution(&dev, VL53L5CX_RESOLUTION_8X8);
    usleep(DELAY);
    status = vl53l5cx_start_ranging(&dev);
    vl53l5cx_check_data_ready(&dev, &isReady);
    usleep(DELAY);
    return 0;
}

void VL53L5CX::getRange(){
    status = vl53l5cx_check_data_ready(&dev, &isReady);
    if(isReady){ 
        vl53l5cx_get_ranging_data(&dev, &Results);
        for(int a = 0; a < 64; a++){
			if(a % 8 == 0) std::cout << "\n";
            std::cout <<"|" << std::setw(4) << 
                    Results.distance_mm[VL53L5CX_NB_TARGET_PER_ZONE*a] << " mm|";
		}
    }
}

void VL53L5CX::getVisualRange(){
    status = vl53l5cx_check_data_ready(&dev, &isReady);
    if(isReady){ 
        vl53l5cx_get_ranging_data(&dev, &Results);
        for(int a = 0; a < 64; a++){
			if(a % 8 == 0) std::cout << "\n";
            if(Results.distance_mm[VL53L5CX_NB_TARGET_PER_ZONE*a] < 300){
                std::cout << RED_STRIPE <<  std::setw(6) << 
                    Results.distance_mm[VL53L5CX_NB_TARGET_PER_ZONE*a] << RESET;
            }else if(Results.distance_mm[VL53L5CX_NB_TARGET_PER_ZONE*a] < 600){
                std::cout << MAGENTA_STRIPE <<  std::setw(6) << 
                    Results.distance_mm[VL53L5CX_NB_TARGET_PER_ZONE*a] << RESET;
            }
            else if(Results.distance_mm[VL53L5CX_NB_TARGET_PER_ZONE*a] < 900){
                std::cout << YELLOW_STRIPE <<  std::setw(6) << 
                    Results.distance_mm[VL53L5CX_NB_TARGET_PER_ZONE*a] << RESET;
            }
            else if(Results.distance_mm[VL53L5CX_NB_TARGET_PER_ZONE*a] < 1200){
                std::cout << GREEN_STRIPE <<  std::setw(6) << 
                    Results.distance_mm[VL53L5CX_NB_TARGET_PER_ZONE*a] << RESET;
            }
            else if(Results.distance_mm[VL53L5CX_NB_TARGET_PER_ZONE*a] < 1400){
                std::cout << CYAN_STRIPE <<  std::setw(6) << 
                    Results.distance_mm[VL53L5CX_NB_TARGET_PER_ZONE*a] << RESET;
            }
            else {
                std::cout << BLUE_STRIPE <<  std::setw(6) << 
                    Results.distance_mm[VL53L5CX_NB_TARGET_PER_ZONE*a] << RESET;
            }
            
		}
    }
}

void VL53L5CX::reportError(int error, std::string error_info) { 
	std::cerr << "Error! " << error_info << ": " << strerror(error); 
}
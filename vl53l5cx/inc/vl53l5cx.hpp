#ifndef __VL53L5CX_HPP__
#define __VL53L5CX_HPP__

#include <memory>
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <dlfcn.h>
#include <string.h>

#include "vl53l5cx_api.hpp"
#include "vl53l5cx_buffers.hpp"

#define DELAY 10000

class VL53L5CX{
public:
    VL53L5CX();
    ~VL53L5CX();
    int init();
    void getRange();
    void reportError(int error, std::string error_info = "Errno");
private:
    VL53L5CX_Configuration dev;
    uint8_t status, isAlive, isReady;
    VL53L5CX_ResultsData Results;
};

#endif
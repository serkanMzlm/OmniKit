#ifndef COMMS_TYPES_HPP
#define COMMS_TYPES_HPP

enum class CommType {
    UART,
    UDP,
    CAN,
    I2C,
    SPI,
    UNKNOWN
};

#endif // COMMS_TYPES_HPP
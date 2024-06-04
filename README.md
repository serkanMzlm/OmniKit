## SENSORS DRIVERS

- Vl53l5cx
- MPU6050
- Optical Flow
- RFID
- Baro
- GPIO
    ```bash
    echo 79 > /sys/class/gpio/export
    echo out > /sys/class/gpio/gpio79/direction
    echo 1 > /sys/class/gpio/gpio79/value
    echo 79 > /sys/class/gpio/unexport
    ```

### VL53L5CX Linux Drivers
https://www.st.com/en/embedded-software/stsw-img025.html#featured_resources-0

### Build Project
```
cmake -DCMAKE_INSTALL_PREFIX=../install ..
make 
make install
```

When entering the command `cmake -DCMAKE_INSTALL_PREFIX=../install ..`, the reason for doing so is to specify the directory where we want to install the file. If not specified, installation will default to the `/usr/local/lib/` directory, requiring root permissions during make install. Otherwise, you'll encounter a **Permission denied** error.
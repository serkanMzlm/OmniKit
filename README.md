### SENSORS

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


https://www.st.com/en/embedded-software/stsw-img025.html#featured_resources-0

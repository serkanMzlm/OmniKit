## vl53l5cx
Bu projede amacım, VL53L5CX mesafe sensörünü Linux tabanlı bir işletim sistemi üzerinde kullanmak ve ardından ROS 2 (Robot Operating System 2) çerçevesi ile entegre etmektir. **Bu proje Raspberry Pi 3 ve Jetson Nano kartlarında denenmiştir.** 

### Kullanılan Platformlar
- Ubuntu 18.04 - Ubuntu 20.04 
- Galactic     - Humble

### Sensor
- Sensör, 2 cm ile 400 cm arasında mesafeleri ölçebilen bir mesafe sensörüdür. 
- Sensör, 8x8 veya 4x4 modunda kullanılabilir.
- 4x4 modunda 60 Hz frekansta çalışabilir.
- I2C iletişim protokolü kullanılır ve çalışma frekansı 400 kHz veya 1 MHz olmalıdır (8x8 modunda 1 MHz olmalıdır).
- Gerekli kütüphaneler [vl53l5cx_linux_driver](https://www.st.com/en/embedded-software/stsw-img025.html#get-software) adresinden indirilebilir. İhtiyaç duyulan kütüphaneler: 
    - platform.c
    - platform.h
    - vl53l5cx_api.h
    - vl53l5cx_api.c
    - vl53l5cx_buffers.h
- ".h" uzantısına sahip olan dosyaları include/vl53l5cx dizinine yerleştirmelisiniz.
- ".c" uzantısına sahip olan dosyaları ise src/vl53l5cx dizinine yerleştirmelisiniz. 
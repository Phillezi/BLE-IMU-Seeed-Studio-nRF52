#pragma once
#ifndef IMU_HPP
#define IMU_HPP

#include "LSM6DS3.h"
#include "Wire.h"

#define DEFAULT_IMU_I2C_ADDR 0x6A
#define DATA_STR_LEN 70

class IMU
{
public:
    // Constructor with a default I2C address
    IMU(uint8_t address = DEFAULT_IMU_I2C_ADDR); // Default I2C address for LSM6DS3

    // Initialize the sensor
    // samplerate is the samplerate in hertz
    bool begin(uint16_t samplerate);

    // Detect significant motion based on acceleration threshold
    inline bool isMotionDetected(float aX,
                                 float aY,
                                 float aZ,
                                 float threshold = 2.5); // Detect motion (default threshold: 2.5)

    void collectAndTransmit(std::function<void(void *, const char *)> transmitCallback, void *callbackArg = nullptr);

private:
    LSM6DS3 sensor; // IMU sensor object

    // Accelerometer and Gyroscope data
};

#endif

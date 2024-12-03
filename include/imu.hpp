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

    void collectAndTransmit(std::function<void(void *, const char *)> transmitCallback, void *callbackArg = nullptr);

    void update(std::function<void(void *, const char *)> transmitCallback, void *callbackArg = nullptr);

private:
    LSM6DS3 sensor; // IMU sensor object

    uint32_t samplesRead;

    uint32_t samplesToRead;

    unsigned long lastSampleRead;

    unsigned long sampleInterval;

    // Accelerometer and Gyroscope data
};

#endif

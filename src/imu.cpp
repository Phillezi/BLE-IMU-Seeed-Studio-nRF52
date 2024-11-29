#include "imu.hpp"

// Constructor: Initializes the IMU with the given I2C address
IMU::IMU(uint8_t address) : sensor(I2C_MODE, address)
{
}

// Initialize the sensor
bool IMU::begin()
{
    if (sensor.begin() != 0)
    {
        Serial.println("Device error");
        return false;
    }
    else
    {
        Serial.println("IMU sensor configured");
        Serial.println("IMU FORMAT:");
        Serial.println("aX,aY,aZ,gX,gY,gZ");
        return true;
    }
}

// Detect motion if the acceleration sum exceeds the threshold
bool IMU::isMotionDetected(float threshold)
{
    aX = sensor.readFloatAccelX();
    aY = sensor.readFloatAccelY();
    aZ = sensor.readFloatAccelZ();

    float aSum = fabs(aX) + fabs(aY) + fabs(aZ);
    return aSum >= threshold;
}

void IMU::collectAndTransmit(std::function<void(void *, String)> transmitCallback, void *callbackArg)
{
    if (isMotionDetected(1.0)) // Threshold for motion detection
    {
        aX = sensor.readFloatAccelX();
        aY = sensor.readFloatAccelY();
        aZ = sensor.readFloatAccelZ();
        gX = sensor.readFloatGyroX();
        gY = sensor.readFloatGyroY();
        gZ = sensor.readFloatGyroZ();

        // Format the data as CSV
        // first three is acceleration and the last three are gyro.
        String dataLine = String(aX, 3) + "," + String(aY, 3) + "," + String(aZ, 3) + "," +
                          String(gX, 3) + "," + String(gY, 3) + "," + String(gZ, 3);

        transmitCallback(callbackArg, dataLine);

        Serial.println(dataLine);
    }
}
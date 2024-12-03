#include "imu.hpp"

// Constructor: Initializes the IMU with the given I2C address
IMU::IMU(uint8_t address) : sensor(I2C_MODE, address)
{
}

// Initialize the sensor
bool IMU::begin(uint16_t samplerate)
{
    sensor.settings.gyroSampleRate = samplerate;
    sensor.settings.accelSampleRate = samplerate;
    if (sensor.begin() != 0)
    {
        Serial.println("Device error");
        return false;
    }
    else
    {
        Serial.println("ACCELERATION sample rate");
        Serial.print(sensor.settings.accelSampleRate);
        Serial.println("Hz");
        Serial.println("GYRO sample rate");
        Serial.print(sensor.settings.gyroSampleRate);
        Serial.println("Hz");
        Serial.println("IMU sensor configured");
        Serial.println("IMU FORMAT:");
        Serial.println("aX,aY,aZ,gX,gY,gZ");
        return true;
    }
}

// Detect motion if the acceleration sum exceeds the threshold
inline bool IMU::isMotionDetected(
    float aX,
    float aY,
    float aZ,
    float threshold)
{

    float aSum = fabs(aX) + fabs(aY) + fabs(aZ);
    return aSum >= threshold;
}

void IMU::collectAndTransmit(std::function<void(void *, const char *)> transmitCallback, void *callbackArg)
{
    float aX = sensor.readFloatAccelX();
    float aY = sensor.readFloatAccelY();
    float aZ = sensor.readFloatAccelZ();

    /*if (isMotionDetected(aX, aY, aZ, 1.0)) // Threshold for motion detection
    {*/
    float gX = sensor.readFloatGyroX();
    float gY = sensor.readFloatGyroY();
    float gZ = sensor.readFloatGyroZ();

    char dataBuffer[DATA_STR_LEN + 1] = {0};

    // Format the data as CSV
    // first three is acceleration and the last three are gyro.
    snprintf(dataBuffer, DATA_STR_LEN, "%.3f,%.3f,%.3f,%.3f,%.3f,%.3f", aX, aY, aZ, gX, gY, gZ);
    dataBuffer[DATA_STR_LEN] = 0;

    transmitCallback(callbackArg, dataBuffer);

    Serial.println(dataBuffer);
    //}
}
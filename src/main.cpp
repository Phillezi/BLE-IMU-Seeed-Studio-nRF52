#include <Arduino.h>
#include "ble.hpp"
#include "imu.hpp"

#define SERVICE_UUID "123e4567-e89b-12d3-a456-426614174000"
#define IMU_CHARACTERISTIC_UUID "123e4567-e89b-12d3-a456-426614174001"

IMU imu(0x6A);
BLEServer server(SERVICE_UUID, IMU_CHARACTERISTIC_UUID);

void transmitCallback(void *arg, String data)
{
    BLEServer *pServer = (BLEServer *)arg;
    pServer->transmit(data);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(50); // Briefly flash the LED
    digitalWrite(LED_BUILTIN, LOW);
}

void setup()
{
    Serial.begin(SERIAL_BAUDRATE);

    // Initialize BLE server
    if (!server.begin())
    {
        Serial.println("Failed to initialize BLE server");
        while (true)
            ; // Halt execution
    }

    // Initialize IMU
    if (!imu.begin())
    {
        Serial.println("Failed to initialize IMU");
        while (true)
            ; // Halt execution
    }

    Serial.println("Initialization successful!");
}

void loop()
{
    // Collect IMU data and transmit over BLE
    imu.collectAndTransmit(transmitCallback, (void *)&server);

    delay(10);
}

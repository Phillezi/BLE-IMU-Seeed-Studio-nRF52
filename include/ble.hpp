#pragma once
#ifndef BLE_HPP
#define BLE_HPP

#include <ArduinoBLE.h>
#include <functional>

#define UPDATE_FREQ_MS 1000
#define SERIAL_BAUDRATE 9600

class BLEServer
{
public:
    // Constructor: Initializes the BLE service
    BLEServer(const char *serviceUUID, const char *characteristicUUID);

    // Initialize BLE and service
    bool begin();

    void update();

    void transmit(String data);

private:
    // BLE Service and Characteristics
    BLEService bleService;
    BLECharacteristic dataCharacteristic; // Characteristic for sending sensor data

    BLEDevice central;
};

#endif // BLE_HPP

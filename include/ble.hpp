#pragma once
#ifndef BLE_HPP
#define BLE_HPP

#include <ArduinoBLE.h>
#include <functional>

#define SERIAL_BAUDRATE 9600
#define DATA_STR_LEN 70

class BLEServer
{
public:
    // Constructor: Initializes the BLE service
    BLEServer(const char *serviceUUID, const char *characteristicUUID);

    // Initialize BLE and service
    bool begin();

    void update();

    void transmit(const char *data);

    bool isConnected()
    {
        update();
        return central.connected();
    }

private:
    // BLE Service and Characteristics
    BLEService bleService;
    BLECharacteristic dataCharacteristic; // Characteristic for sending sensor data

    BLEDevice central;
};

#endif // BLE_HPP

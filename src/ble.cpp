#include "ble.hpp"

// Constructor
BLEServer::BLEServer(
    const char *serviceUUID, const char *characteristicUUID)
    : bleService(serviceUUID),
      dataCharacteristic(characteristicUUID, BLERead | BLENotify, 0)
{
}

// Initialize BLE Server
bool BLEServer::begin()
{
    Serial.begin(SERIAL_BAUDRATE);
    while (!Serial)
        ;

    if (!BLE.begin())
    {
        Serial.println("BLE initialization failed!");
        return false;
    }

    // Set BLE peripheral device details
    BLE.setLocalName("XIAO IMU BLE");
    BLE.setAdvertisedService(bleService);

    // Add data characteristic to the service (for sending IMU data)
    bleService.addCharacteristic(dataCharacteristic);

    // Add the service to the BLE stack
    BLE.addService(bleService);

    // Start advertising the BLE service
    BLE.advertise();

    // Print device address
    Serial.print("Device Address: ");
    Serial.println(BLE.address());

    Serial.println("XIAO nRF52840 BLE Peripheral");

    // Listen for BLE central connections
    central = BLE.central();

    return true;
}

void BLEServer::transmit(String data)
{
    if (central)
    {
        if (central.connected())
        {
            Serial.print("sent data to: ");
            Serial.println(central.address());
            dataCharacteristic.setValue(data.c_str());
        }
    }
}

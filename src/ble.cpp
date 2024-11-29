#include "ble.hpp"

// Constructor
BLEServer::BLEServer(
    const char *serviceUUID, const char *characteristicUUID)
    : bleService(serviceUUID),
      dataCharacteristic(characteristicUUID, BLERead | BLENotify, DATA_STR_LEN)
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

// Handle BLE connections and disconnections
void BLEServer::update()
{
    BLEDevice newCentral = BLE.central();

    // Handle new central connection
    if (newCentral && !central)
    {
        central = newCentral;
        Serial.print("Connected to central: ");
        Serial.println(central.address());
    }

    // Handle central disconnection
    if (central && !central.connected())
    {
        Serial.print("Disconnected from central: ");
        Serial.println(central.address());
        central = BLEDevice(); // Reset central
    }
}

void BLEServer::transmit(const char *data)
{
    if (central)
    {
        if (central.connected())
        {
            Serial.print("sent data to: ");
            Serial.println(central.address());
            dataCharacteristic.setValue(data);
        }
    }
}

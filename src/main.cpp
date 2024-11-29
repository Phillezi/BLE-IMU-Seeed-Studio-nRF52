#include "ble.hpp"
#include "imu.hpp"

#define SERVICE_UUID "123e4567-e89b-12d3-a456-426614174000"
#define IMU_CHARACTERISTIC_UUID "123e4567-e89b-12d3-a456-426614174001"

void transmitCallback(void *arg, String data)
{
    BLEServer *pServer = (BLEServer *)arg;
    pServer->transmit(data);
}

int main()
{
    IMU imu(0x6A);

    BLEServer server(SERVICE_UUID, IMU_CHARACTERISTIC_UUID);
    if (!server.begin() ||
        !imu.begin())
    {
        return -1;
    }

    while (true)
    {
        imu.collectAndTransmit(transmitCallback, (void *)&server);
    }
    return 0;
}

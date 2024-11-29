#include <Arduino.h>
#include "ble.hpp"
#include "imu.hpp"
#include "rtc.hpp"
#include "nblink.hpp"

#include <nrf_power.h>
#include <nrf_soc.h>

#define SERVICE_UUID "123e4567-e89b-12d3-a456-426614174000"
#define IMU_CHARACTERISTIC_UUID "123e4567-e89b-12d3-a456-426614174001"

IMU imu(DEFAULT_IMU_I2C_ADDR);
BLEServer server(SERVICE_UUID, IMU_CHARACTERISTIC_UUID);

void transmitCallback(void *arg, const char *data)
{
    BLEServer *pServer = (BLEServer *)arg;
    pServer->update();

    pServer->transmit(data);

    nBlink(); // blink the led in a non-blocking way
}

void enterSleepMode()
{
    digitalWrite(LED_BUILTIN, LOW);
    Serial.println("Entering sleep mode.");

    if (!server.isConnected())
    {
        __SEV(); // Clear any pending events
        __WFE();
        __WFE(); // Enter System ON sleep mode (requires two calls to __WFE)
    }

    Serial.println("Waking up from sleep.");
    digitalWrite(LED_BUILTIN, HIGH);
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

    setupRTC();

    Serial.println("Initialization successful!");
}

void loop()
{
    if (server.isConnected())
    {
        enableRTC();
        if (ticker)
        {
            ticker = false;
            // Collect IMU data and transmit over BLE
            imu.collectAndTransmit(transmitCallback, (void *)&server);
        }
        else
        {
            yield();
        }
    }
    else
    {
        disableRTC();
        enterSleepMode();
    }
}

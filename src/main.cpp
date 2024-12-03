#include <Arduino.h>
#include "ble.hpp"
#include "imu.hpp"
#include "rtc.hpp"
#include "nblink.hpp"

#include <nrf_power.h>
#include <nrf_soc.h>

#define SERVICE_UUID "123e4567-e89b-12d3-a456-426614174000"
#define IMU_CHARACTERISTIC_UUID "123e4567-e89b-12d3-a456-426614174001"

#define IMU_SAMPLE_RATE_HZ 240

// uncomment to enable RTC, It doesnt work currently
// #define USE RTC

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
    if (!imu.begin(IMU_SAMPLE_RATE_HZ))
    {
        Serial.println("Failed to initialize IMU");
        while (true)
            ; // Halt execution
    }

#ifdef USE_RTC
    setupRTC();
#endif

    Serial.println("Initialization successful!");
}

void loop()
{
    static unsigned long lasttransmit = 0;
    if (server.isConnected())
    {
#ifdef USE_RTC
        enableRTC();
#endif
        if (
#ifndef USE_RTC
            true
#else
            ticker
#endif
        )
        {
#ifdef USE_RTC
            ticker = false;
#endif
            //  Collect IMU data and transmit over BLE
            imu.collectAndTransmit(transmitCallback, (void *)&server);
        }
        else
        {
            yield();
        }
    }
    else
    {
        digitalWrite(LED_BUILTIN, LOW);
#ifdef USE_RTC
        disableRTC();
#endif
        // enterSleepMode();
        yield();
    }
}

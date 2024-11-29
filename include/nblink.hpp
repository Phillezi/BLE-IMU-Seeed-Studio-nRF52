#pragma once
#ifndef N_BLINK_HPP
#define N_BLINK_HPP

#include <Arduino.h>

#define BLINK_INTERVAL_MS 50

void nBlink()
{
    static unsigned long lastFlashTime = 0; // Keep track of the last time the LED state was changed
    static bool ledState = false;           // Current state of the LED

    unsigned long currentTime = millis();

    if (!ledState)
    {
        digitalWrite(LED_BUILTIN, HIGH);
        ledState = true;
        lastFlashTime = currentTime; // Record the start time
    }

    if (ledState && (currentTime - lastFlashTime >= BLINK_INTERVAL_MS))
    {
        digitalWrite(LED_BUILTIN, LOW);
        ledState = false; // Reset state
    }
}

#endif

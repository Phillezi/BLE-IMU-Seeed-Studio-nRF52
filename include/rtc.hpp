#pragma once
#ifndef RTC_HPP
#define RTC_HPP

#include <nrf_rtc.h>

#define UPDATE_INTERVAL_MS 20

volatile bool ticker = false;

void rtcHandler()
{
    ticker = true;                   // Set flag when RTC triggers
    NRF_RTC2->EVENTS_COMPARE[0] = 0; // Clear the event
}

void enableRTC()
{
    NRF_RTC2->TASKS_START = 1; // Start the RTC
    ticker = true;
}

void disableRTC()
{
    NRF_RTC2->TASKS_STOP = 1; // Stop the RTC
    ticker = false;           // Reset the ticker flag
}

void setupRTC()
{
    NRF_RTC2->TASKS_STOP = 1;  // Stop RTC in case it was running
    NRF_RTC2->TASKS_CLEAR = 1; // Clear RTC counter

    NRF_RTC2->PRESCALER = 32;             // Set prescaler to 32 (1 tick = ~1ms)
    NRF_RTC2->CC[0] = UPDATE_INTERVAL_MS; // Set compare value for interval

    NRF_RTC2->INTENSET = RTC_INTENSET_COMPARE0_Msk; // Enable compare interrupt
    NVIC_EnableIRQ(RTC2_IRQn);                      // Enable RTC interrupt in NVIC
}

extern "C" void RTC2_IRQHandler()
{
    if (NRF_RTC2->EVENTS_COMPARE[0])
    {
        rtcHandler(); // Call handler when RTC interrupt fires
    }
}

#endif

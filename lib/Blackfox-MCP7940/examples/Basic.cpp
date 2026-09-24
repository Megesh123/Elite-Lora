#include "MCP7940.h"
#include <Arduino.h>

MCP7940 rtc;

#define Date 15
#define Month 8
#define Year 2025
#define Hour 14
#define Minute 30
#define Second 0
#define Weekday 5

bool _isRtcInit = false;

void setup()
{
    Serial.begin(115200);

    // Option 1: Default init
    rtc.init();

    // Option 2: Custom address
    // rtc.init(0x6F);

    rtc.setDateTime(_isRtcInit,Date, Month, Year, Hour, Minute, Second, Weekday);
}

void loop()
{
    DateTime now = rtc.getDateTime();
    Serial.printf("Date > %02d:%02d:%04d \nTime > %02d:%02d:%02d \n", now.date, now.month, now.year, now.hour, now.minute, now.second);
    delay(1000);
}
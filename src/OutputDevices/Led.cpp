/**
 * @file Main.cpp
 * @author Megeshwaran D <megesh@bfes.co.in>
 * @version 1.0.0
 * @date 2025-10-01
 * @copyright Copyright (c) 2025 [Blackfox Embedded Solutions]
 *
 * @note LED indicator control implementation
 */

/**********************
 * LED INDICATOR CONTROL IMPLEMENTATION
 * Provides visual feedback through LED indicators for system status and alerts
 * Implements various blinking patterns for different fault conditions
 **********************/

#include "Headerfile.h" // Include main system header

/**********************
 * ALL LEDs ON
 * Turns on all LED indicators for full status display
 **********************/
void LEDFUNCTION::on()
{
    LedFun.state = 0;
}

/**********************
 * ALL LEDs OFF
 * Turns off all LED indicators
 **********************/
void LEDFUNCTION::off()
{
    LedFun.state = 1;
}

/**********************
 * OVERLOAD INDICATION
 * Visual indication for electrical overload condition
 **********************/
void LEDFUNCTION::overLoad()
{
    if (!RefillStart && !RefillStop && !RefillCompleted) // If not in refill start or stop mode
    {
        LedFun.state = 2;
    }
}

/**********************
 * SUPERVISORY MODE INDICATION
 * Visual indication for supervisory or monitoring mode
 **********************/
void LEDFUNCTION::Supervisory()
{
    if (!RefillStart && !RefillStop && !RefillCompleted) // If not in refill start or stop mode
    {
        LedFun.state = 3;
    }
}

/**********************
 * RESERVOIR LOW LEVEL INDICATION
 * Visual indication for low reservoir level condition
 **********************/
void LEDFUNCTION::reservoir()
{
    if (!RefillStart && !RefillStop && !RefillCompleted) // If not in refill start or stop mode
    {
        LedFun.state = 4;
    }
}

/**********************
 * PROXIMITY SENSOR NOT SENSING INDICATION
 * Visual indication for proximity sensor not sensing condition
 **********************/
void LEDFUNCTION::proximityNotSense()
{
    if (!RefillStart && !RefillStop && !RefillCompleted) // If not in refill start or stop mode
    {
        LedFun.state = 5;
    }
}

/**********************
 * TEST RUN ON INDICATION
 * Visual indication for test run mode activation
 **********************/
void LEDFUNCTION::Testrun_on()
{
    LedFun.state = 6;
}

/**********************
 * TEST RUN OFF INDICATION
 * Visual indication for test run mode deactivation
 **********************/
void LEDFUNCTION::Testrun_off()
{
    LedFun.state = 1;
}

bool led_state = true;

void LEDFUNCTION::_on()
{
    led_state = !led_state;
    if (led_state)
    {
        digitalWrite(PIN_LED_COMMUNICATION, HIGH);
        digitalWrite(PIN_LED_OVERLOAD_REFILL, LOW);
        digitalWrite(PIN_LED_SLEEP_FAULT, LOW);
        digitalWrite(PIN_LED_TEST_RUN, LOW);
    }
    else
    {
        digitalWrite(PIN_LED_COMMUNICATION, LOW);
        digitalWrite(PIN_LED_OVERLOAD_REFILL, HIGH);
        digitalWrite(PIN_LED_SLEEP_FAULT, HIGH);
        digitalWrite(PIN_LED_TEST_RUN, HIGH);
    }
}

void LEDFUNCTION::_off()
{
    digitalWrite(PIN_LED_COMMUNICATION, LOW);
    digitalWrite(PIN_LED_OVERLOAD_REFILL, LOW);
    digitalWrite(PIN_LED_SLEEP_FAULT, LOW);
    digitalWrite(PIN_LED_TEST_RUN, LOW);
}

void LEDFUNCTION::_overload()
{
    digitalWrite(PIN_LED_COMMUNICATION, LOW);
    digitalWrite(PIN_LED_OVERLOAD_REFILL, HIGH);
    digitalWrite(PIN_LED_SLEEP_FAULT, HIGH);
    digitalWrite(PIN_LED_TEST_RUN, LOW);
}

void LEDFUNCTION::_reservoir()
{
    led_state = !led_state;
    if (led_state)
    {
        digitalWrite(PIN_LED_COMMUNICATION, HIGH);
        digitalWrite(PIN_LED_OVERLOAD_REFILL, LOW);
        digitalWrite(PIN_LED_SLEEP_FAULT, HIGH);
        digitalWrite(PIN_LED_TEST_RUN, LOW);
    }
    else
    {
        digitalWrite(PIN_LED_COMMUNICATION, LOW);
        digitalWrite(PIN_LED_OVERLOAD_REFILL, LOW);
        digitalWrite(PIN_LED_SLEEP_FAULT, HIGH);
        digitalWrite(PIN_LED_TEST_RUN, LOW);
    }
}

void LEDFUNCTION::_proximity()
{
    digitalWrite(PIN_LED_COMMUNICATION, LOW);
    digitalWrite(PIN_LED_OVERLOAD_REFILL, LOW);
    digitalWrite(PIN_LED_SLEEP_FAULT, HIGH);
    digitalWrite(PIN_LED_TEST_RUN, LOW);
}

void LEDFUNCTION::_supervisory()
{
    led_state = !led_state;
    if (led_state)
    {
        digitalWrite(PIN_LED_COMMUNICATION, HIGH);
        digitalWrite(PIN_LED_SLEEP_FAULT, LOW);
        digitalWrite(PIN_LED_OVERLOAD_REFILL, HIGH);
        digitalWrite(PIN_LED_TEST_RUN, LOW);
    }
    else
    {
        digitalWrite(PIN_LED_COMMUNICATION, LOW);
        digitalWrite(PIN_LED_SLEEP_FAULT, HIGH);
        digitalWrite(PIN_LED_OVERLOAD_REFILL, LOW);
        digitalWrite(PIN_LED_TEST_RUN, LOW);
    }
}

void LEDFUNCTION::_testrun()
{
    digitalWrite(PIN_LED_COMMUNICATION, LOW);
    digitalWrite(PIN_LED_TEST_RUN, HIGH);
    digitalWrite(PIN_LED_SLEEP_FAULT, LOW);
    digitalWrite(PIN_LED_OVERLOAD_REFILL, LOW);
}
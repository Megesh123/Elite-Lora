#include "Headerfile.h"

TestingFunction Testing;

bool voltageTestResult = false;
bool currentTestResult = false;
bool proximityTestResult = false;
bool limit_1SwitchTestResult = false;
bool limit_2SwitchTestResult = false;
bool limit_3SwitchTestResult = false;
bool pumpTestResult = false;
bool buzzerTestResult = false;
bool AlreadyTestedBoard = false;
bool volt_current_test_done = false;
bool TestingModeEnable = false;
bool MenuButtonTestResult = false;
bool BackButtonTestResult = false;
bool UpButtonTestResult = false;
bool DownButtonTestResult = false;
bool OkButtonTestResult = false;

void TestingFunction::Mode()
{
    if (!volt_current_test_done)
    {
        Motor.on();
        delay(100);
        Testing.voltageTest();
        Testing.currentTest();
    }
    Testing.proximityTest();
    Testing.limitSwitchTest();
    Testing.menuButtonTest();
    Testing.backButtonTest();
    Testing.upButtonTest();
    Testing.downButtonTest();
    Testing.okButtonTest();
    float testVoltage = Voltage.Read();
    if (testVoltage < VoltageMonitor.calibration)
    {
        if (MenuButtonTestResult && BackButtonTestResult && UpButtonTestResult && DownButtonTestResult && OkButtonTestResult && voltageTestResult && currentTestResult && proximityTestResult && limit_1SwitchTestResult && limit_2SwitchTestResult && limit_3SwitchTestResult && pumpTestResult && buzzerTestResult)
        {
            AlreadyTestedBoard = true;
        }
        Serial.println("Testing Completed.");
        Eeprom.write();
        delay(1000);
    }

    delay(1);
}

void TestingFunction::voltageTest()
{
    float testVoltage = Voltage.Read();
    delay(10);
    if (testVoltage < 12.0 || testVoltage > 30.0)
    {
        if (!_onces_print_volt_test)
        {
            _onces_print_buzzer_test = true;
            Serial.print(" Volt- Fail,");
        }
    }
    else
    {
        if (!_onces_print_volt_test)
        {
            _onces_print_volt_test = true;
            Serial.print(" Volt- Pass,");
        }
        voltageTestResult = true;
    }
}
void TestingFunction::currentTest()
{
    float testCurrent = Current.Read();
    delay(10);

    if (testCurrent >= 0.2)
    {
        if (!_onces_print_curr_test)
        {
            _onces_print_curr_test = true;
            Serial.print(" Current- Pass,");
            Serial.print(" Pump- Pass,");
        }
    }
    else
    {
        if (!_onces_print_curr_test)
        {
            _onces_print_curr_test = true;
            Serial.print(" Current- Fail,");
            Serial.print(" Pump- Fail,");
        }
    }
    currentTestResult = true;
    pumpTestResult = true;
    volt_current_test_done = true;
}

void TestingFunction::proximityTest()
{

    if (digitalRead(PIN_PROXIMITY_SENSOR) == Pressed)
    {
        if (!_onces_print_prox_test)
        {
            _onces_print_prox_test = true;
            Serial.print(" Proximity- Pass,");
            digitalWrite(PIN_BUZZER, HIGH);
        }
        proximityTestResult = true;
    }
    else
    {
        digitalWrite(PIN_BUZZER, LOW);
    }
}

void TestingFunction::limitSwitchTest()
{
    if (digitalRead(PIN_LIMIT_SW_1) == Pressed)
    {
        if (!_onces_print_limit_1_sw_test)
        {
            _onces_print_limit_1_sw_test = true;
            Serial.print(" LS_1- Pass,");
        }
        limit_1SwitchTestResult = true;
    }

    if (digitalRead(PIN_LIMIT_SW_2) == Pressed)
    {
        if (!_onces_print_limit_2_sw_test)
        {
            _onces_print_limit_2_sw_test = true;
            Serial.print(" LS_2- Pass,");
        }
        limit_2SwitchTestResult = true;
    }
    if (digitalRead(PIN_LIMIT_SW_3) == Pressed)
    {
        if (!_onces_print_limit_3_sw_test)
        {
            _onces_print_limit_3_sw_test = true;
            Serial.print(" LS_3- Pass,");
        }
        limit_3SwitchTestResult = true;
    }
}

void TestingFunction::menuButtonTest()
{
    if (Button.menu.isPressed() && !_button_menu_pressed)
    {
        if (!_onces_print_menu_button_test)
        {
            _onces_print_menu_button_test = true;
            Serial.print(" Button_Menu- Pass,");
            Serial.print(" Buzzer- Pass,");
        }
        _button_menu_pressed = true;
        buzzerTestResult = true;
        MenuButtonTestResult = true;
    }

    if (Button.menu.isNotPressed())
    {
        _button_menu_pressed = false;
    }
}

void TestingFunction::backButtonTest()
{
    if (Button.back.isPressed() && !_button_back_pressed)
    {
        if (!_onces_print_back_button_test)
        {
            _onces_print_back_button_test = true;
            Serial.print(" Button_Back- Pass,");
        }
        _button_back_pressed = true;
        BackButtonTestResult = true;
    }

    if (Button.back.isNotPressed())
    {
        _button_back_pressed = false;
    }
}

void TestingFunction::upButtonTest()
{
    if (Button.up.isPressed() && !_button_up_pressed)
    {
        if (!_onces_print_up_button_test)
        {
            _onces_print_up_button_test = true;
            Serial.print(" Button_Up- Pass,");
        }
        _button_up_pressed = true;
        UpButtonTestResult = true;
    }

    if (Button.up.isNotPressed())
    {
        _button_up_pressed = false;
    }
}

void TestingFunction::downButtonTest()
{
    if (Button.down.isPressed() && !_button_down_pressed)
    {
        if (!_onces_print_buzzer_test)
        {
            _onces_print_buzzer_test = true;
            Serial.print(" Button_Down- Pass,");
        }
        _button_down_pressed = true;
        DownButtonTestResult = true;
    }

    if (Button.down.isNotPressed())
    {
        _button_down_pressed = false;
    }
}

void TestingFunction::okButtonTest()
{
    if (Button.ok.isPressed() && !_button_ok_pressed)
    {
        if (!_onces_print_ok_button_test)
        {
            _onces_print_ok_button_test = true;
            Serial.print(" Button_OK- Pass,");
        }
        _button_ok_pressed = true;
        OkButtonTestResult = true;
    }

    if (Button.ok.isNotPressed())
    {
        _button_ok_pressed = false;
    }
}
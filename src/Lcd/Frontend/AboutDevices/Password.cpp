#include "HeaderFile.h"
#include "esp_task_wdt.h"

void ADMINSETTING::DisplayEnterPassword()
{
    display.clearDisplay();
    display.setCursor(25, 20);
    display.print(PRINT_ENTER_PASSWORD);
}

void ADMINSETTING::DisplayWorngPassword()
{
    Alert.Total.WorngPassword++;
    display.clearDisplay();
    display.setCursor(25, 30);
    display.print(PRINT_WRONG_PASSWORD);
    display.display();
    vTaskDelay(pdMS_TO_TICKS(3000)); // Yield to other tasks with a delay of 10 milliseconds
}
void ADMINSETTING::FactoryReset()
{
    int _AboutDeviceSetcursor = AboutDeviceSetcursor; // Local copy of cursor position
    if (Button.ok.isPressed() && _AboutDeviceSetcursor == 17 * 12)
    {
        bool _Pressed = true;
        display.clearDisplay();
        LcdDisplay.Boder(); // Draw the border on the display
        display.setCursor(10, 30);
        display.print(PRINT_DO_YOU_NEED_RESET);
        display.display();
        Button.ok.wait();
        while (_Pressed)
        {
            display.clearDisplay();
            LcdDisplay.Boder(); // Draw the border on the display
            display.setCursor(10, 30);
            display.print(PRINT_DO_YOU_NEED_RESET);

            LcdDisplay.printOk();

            display.display();
            if (Button.ok.isPressed())
            {
                Config.NoOfOutlet = -1;
                Eeprom.init();
                for (int i = 0; i < 3; i++)
                {
                    display.clearDisplay();
                    LcdDisplay.Boder(); // Draw the border on the display
                    display.setCursor(20, 30);
                    display.print(PRINT_RESET_COMPLETED);
                    display.display();
                    vTaskDelay(pdMS_TO_TICKS(600)); // Yield to other tasks with a delay of 10 milliseconds
                    display.clearDisplay();
                    LcdDisplay.Boder(); // Draw the border on the display
                    display.display();
                    vTaskDelay(pdMS_TO_TICKS(400)); // Yield to other tasks with a delay of 10 milliseconds
                }

                _Pressed = false;
                Button.ok.wait();
                ESP.restart(); // Restart system to apply new configuration
            }
            if (Button.back.isPressed())
            {
                _Pressed = false;
                Button.back.wait();
            }

            esp_task_wdt_reset();
            vTaskDelay(pdMS_TO_TICKS(1)); // Yield to other tasks with a delay of 10 milliseconds
        }
    }
}

void ADMINSETTING::System()
{
    int _AboutDeviceSetcursor = AboutDeviceSetcursor;
    if (Button.ok.isPressed() && _AboutDeviceSetcursor == 16 * 12)
    {
        _take_old_sate();
        _System_setting();
        Button.ok.wait();
        _Pressed = true;
        SystemSetcursor = 12;

        while (_Pressed)
        {
            _System_setting();
            _system_up();
            _system_down();
            _system_ok();
            _system_back();
            esp_task_wdt_reset();
            vTaskDelay(pdMS_TO_TICKS(1)); // Yield to other tasks with a delay of 10 milliseconds
        }
    }
}

void ADMINSETTING::_System_setting()
{
    display.clearDisplay();
    LcdDisplay.Boder(); // Draw the border on the display
    if (LcdUI._shouldBlink(ArrowlastBlinkTime, ArrowblinkState))
    {

        display.setCursor(10, SystemSetcursor);
        display.print(PRINT_RIGHT_ARROW);
        LcdDisplay._DrawStraightLine(7, SystemSetcursor - 4, 5, HORIZONTAL);
    }
    if (Alert.EnableDisable.Supervisory1)
    {
        LcdDisplay.print(20, 12, PRINT_SUPERVISORY_1_ENABLE);
    }
    else
    {
        LcdDisplay.print(20, 12, PRINT_SUPERVISORY_1_DISABLE);
    }

    if (Alert.EnableDisable.Supervisory2)
    {
        LcdDisplay.print(20, 24, PRINT_SUPERVISORY_2_ENABLE);
    }
    else
    {
        LcdDisplay.print(20, 24, PRINT_SUPERVISORY_2_DISABLE);
    }

    if (Alert.EnableDisable.Supervisory3)
    {
        LcdDisplay.print(20, 36, PRINT_SUPERVISORY_3_ENABLE);
    }
    else
    {
        LcdDisplay.print(20, 36, PRINT_SUPERVISORY_3_DISABLE);
    }

    if (Alert.EnableDisable.RefillAlert)
    {
        LcdDisplay.print(20, 48, PRINT_REFILL_ALERT_ENABLE);
    }
    else
    {
        LcdDisplay.print(20, 48, PRINT_REFILL_ALERT_DISABLE);
    }

    LcdDisplay.print(20, 60, PRINT_OTA);

    display.display();
}

void ADMINSETTING::_system_up()
{

    if (Button.down.isPressed())
    {
        SystemSetcursor += 12;
        if (SystemSetcursor > 60)
            SystemSetcursor = 12;
        Button.down.wait();
    }
}
void ADMINSETTING::_system_down()
{
    if (Button.up.isPressed())
    {
        SystemSetcursor -= 12;
        if (SystemSetcursor < 12)
            SystemSetcursor = 60;
        Button.up.wait();
    }
}
void ADMINSETTING::_system_ok()
{
    if (Button.ok.isPressed())
    {

        if (SystemSetcursor == 12)
        {
            Alert.EnableDisable.Supervisory1 = !Alert.EnableDisable.Supervisory1;
        }
        else if (SystemSetcursor == 24)
        {
            Alert.EnableDisable.Supervisory2 = !Alert.EnableDisable.Supervisory2;
        }
        else if (SystemSetcursor == 36)
        {
            Alert.EnableDisable.Supervisory3 = !Alert.EnableDisable.Supervisory3;
        }
        else if (SystemSetcursor == 48)
        {
            Alert.EnableDisable.RefillAlert = !Alert.EnableDisable.RefillAlert;
        }
        else if (SystemSetcursor == 60)
        {
            ChechOTAUpdate = true;
            Eeprom.write();
            vTaskDelay(pdMS_TO_TICKS(1));
            ESP.restart();
        }
        Button.ok.wait();
    }
}

void ADMINSETTING::_system_back()
{
    if (Button.back.isPressed())
    {
        _Pressed = false;
        if (oldstate_Supervisory1_Enable != Alert.EnableDisable.Supervisory1)
        {
            GreaseOut.Lubrication.Temp.GramSupervisory_1 = 0;
            SupervisoryTime.SupervisoryNominalTakeOutGrease_1 = 0;
            SupervisoryTime.SupervisoryAverageTakeOutGrease_1 = 0;
        }
        if (oldstate_Supervisory2_Enable != Alert.EnableDisable.Supervisory2)
        {
            GreaseOut.Lubrication.Temp.GramSupervisory_2 = 0;
            SupervisoryTime.SupervisoryNominalTakeOutGrease_2 = 0;
            SupervisoryTime.SupervisoryAverageTakeOutGrease_2 = 0;
        }
        if (oldstate_Supervisory3_Enable != Alert.EnableDisable.Supervisory3)
        {
            GreaseOut.Lubrication.Temp.GramSupervisory_3 = 0;
            SupervisoryTime.SupervisoryNominalTakeOutGrease_3 = 0;
            SupervisoryTime.SupervisoryAverageTakeOutGrease_3 = 0;
        }

        if (oldstate_Supervisory1_Enable != Alert.EnableDisable.Supervisory1 || oldstate_Supervisory2_Enable != Alert.EnableDisable.Supervisory2 || oldstate_Supervisory3_Enable != Alert.EnableDisable.Supervisory3)
        {
            Lora_twp.SupervisoryAlertEnableDisable = true;
        }
        if (oldstate_Refill_Enable != Alert.EnableDisable.RefillAlert)
        {
            Lora_twp.RefillAlertEnableDisable = true;

            if (Alert.EnableDisable.RefillAlert)
            {
                check_refill_status = false;
                GreaseOut.Reservoir.Percentage = 0; // Set reservoir percentage to 0%
                GreaseOut.Reservoir.Temp.Percentage = 0;
                GreaseOut.Reservoir.Temp._old_reservoir_value = -1;
            }
        }
        Button.back.wait();
    }
}

void ADMINSETTING::_take_old_sate()
{
    oldstate_Supervisory1_Enable = Alert.EnableDisable.Supervisory1;
    oldstate_Supervisory2_Enable = Alert.EnableDisable.Supervisory2;
    oldstate_Supervisory3_Enable = Alert.EnableDisable.Supervisory3;
    oldstate_Refill_Enable = Alert.EnableDisable.RefillAlert;
}

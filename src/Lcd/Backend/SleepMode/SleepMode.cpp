#include "HeaderFile.h"

void SLEEPMODE::Enable()
{
    if (Button.back.isPressed() && Button.up.isPressed() && Button.menu.isNotPressed() && Button.down.isNotPressed() && Button.ok.isNotPressed() && MainEnable && !Keylock && !GreaseOut.Sleep.Enable)
    {
        while (Button.back.isPressed())
        {
            if (Button.back.isPressed() && Button.up.isPressed() && Button.menu.isNotPressed() && Button.down.isNotPressed() && Button.ok.isNotPressed() && MainEnable && !Keylock && !GreaseOut.Sleep.Enable)
            {
                SleepModeEnableCount++;
                Button.up.wait();

                if (SleepModeEnableCount > 2)
                {
                    Lora_twp.SleepModeOn = true;
                    SLeepMode.start();
                }
            }
            vTaskDelay(pdMS_TO_TICKS(1)); // Yield to other tasks with a delay of 10 milliseconds
        }
        SleepModeEnableCount = 0;
        SleepModeDisableCount = 0;
    }
}

void SLEEPMODE::Disable()
{

    if (Button.back.isPressed() && Button.down.isPressed() && Button.menu.isNotPressed() && Button.up.isNotPressed() && Button.ok.isNotPressed() && !Keylock && GreaseOut.Sleep.Enable)
    {
        vTaskDelay(pdMS_TO_TICKS(10)); // Yield to other tasks with a delay of 10 milliseconds
        while (Button.back.isPressed())
        {
            if (Button.back.isPressed() && Button.down.isPressed() && Button.menu.isNotPressed() && Button.up.isNotPressed() && Button.ok.isNotPressed() && !Keylock && GreaseOut.Sleep.Enable)
            {
                SleepModeDisableCount++;
                Button.down.wait();

                if (SleepModeDisableCount > 2)
                {
                    if (start_init == 1)
                    {
                        Lora_twp.SleepModeOff = true;
                    }
                    SLeepMode.stop();
                }
            }

            vTaskDelay(pdMS_TO_TICKS(1)); // Yield to other tasks with a delay of 10 milliseconds
        }

        SleepModeEnableCount = 0;
        SleepModeDisableCount = 0;
    }
}

void SLEEPMODE::start()
{
    GreaseOut.Sleep.Enable = true;

    if (GreaseOut.Lubrication.Enable)
    {
        oldStateCount = 1;
    }
    else if (GreaseOut.PauseTime.Enable)
    {
        oldStateCount = 2;
    }
    else if (GreaseOut.TestRun.Enable)
    {
        oldStateCount = 3;
    }
    GreaseOut.Lubrication.Enable = false;
    GreaseOut.PauseTime.Enable = false;
    GreaseOut.TestRun.Enable = false;
    Motor.off();
    Eeprom.write();
}

void SLEEPMODE::stop()
{
    GreaseOut.Sleep.Enable = false;
    if (start_init == 1)
    {
        Grease.begin();
    }
    Eeprom.write();
}
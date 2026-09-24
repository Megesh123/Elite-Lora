#include "HeaderFile.h"

void LcdKeyLockFunction::Enable()
{
    if (KeylockEnable)
    {
        display.clearDisplay();
        LcdDisplay.Boder();
        LcdDisplay.print(25, 36, "KEY LOCK ENABLE");
        display.display();
        vTaskDelay(pdMS_TO_TICKS(3000)); // Yield to other tasks with a delay of 10 milliseconds
        KeylockEnable = false;
    }
}
void LcdKeyLockFunction::Disable()
{
    if (KeylockDisable)
    {
        display.clearDisplay();
        LcdDisplay.Boder();
        LcdDisplay.print(25, 36, "KEY LOCK DISABLE");
        display.display();
        vTaskDelay(pdMS_TO_TICKS(3000)); // Yield to other tasks with a delay of 10 milliseconds
        KeylockDisable = false;
    }
}
void LcdKeyLockFunction::Error()
{
    if (Keylock && (Button.menu.isPressed() || Button.back.isPressed() || Button.up.isPressed() || Button.down.isPressed() || Button.ok.isPressed()))
    {
        display.clearDisplay();
        LcdDisplay.Boder();
        LcdDisplay.print(25, 36, "SET KEY LOCK TO");
        LcdDisplay.print(25, 48, "     'DISABLE'");
        display.display();
        vTaskDelay(pdMS_TO_TICKS(3000)); // Yield to other tasks with a delay of 10 milliseconds

        Button.back.wait();
        Button.ok.wait();
        Button.menu.wait();
        Button.up.wait();
        Button.down.wait();
    }
}

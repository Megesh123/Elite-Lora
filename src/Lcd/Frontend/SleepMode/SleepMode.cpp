#include "HeaderFile.h"
#include "Lcd/Logo/SleepModeLogo1.h"
#include "Lcd/Logo/SleepModeLogo2.h"
#include "Lcd/Logo/SleepModeLogo3.h"

void LcdSleepModeFunction::Page()
{
    display.clearDisplay();
    display.drawBitmap(10, 0, sleepmodelogo1, 70, 70, 1);
    display.display();
    vTaskDelay(pdMS_TO_TICKS(300)); // Yield to other tasks with a delay of 10 milliseconds
    display.clearDisplay();
    display.drawBitmap(10, 0, sleepmodelogo2, 70, 70, 1);
    display.display();
    vTaskDelay(pdMS_TO_TICKS(300)); // Yield to other tasks with a delay of 10 milliseconds
    display.clearDisplay();
    display.drawBitmap(10, 0, sleepmodelogo3, 70, 70, 1);
    display.display();
    vTaskDelay(pdMS_TO_TICKS(300)); // Yield to other tasks with a delay of 10 milliseconds
}
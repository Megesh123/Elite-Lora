#include "Headerfile.h"
#include "esp_task_wdt.h"

void LcdUIFunction::OTAPage()
{
    while (ChechOTAUpdate) // If OTA update is in progress
    {
        display.clearDisplay(); // Clear the display buffer

        if (!ota_process_1)
        {
            LcdDisplay.print(10, 15, "CONNECTING TO WIFI"); // Display "SOFTWARE UPDATE" text
        }
        else
        {
            LcdDisplay.print(10, 15, "CONNECTED TO WIFI"); // Display "SOFTWARE UPDATE" text
        }

        if (ota_process_2)
        {
            LcdDisplay.print(10, 25, "SOFTWARE UPDATE "); // Display "SOFTWARE UPDATE" text
            LcdDisplay.print(10, 40, "PROGRESS: ");       // Display "PROGRESS:" text
            display.print(OTA.progress);                  // Display OTA progress percentage
            display.print(" %  ");                        // Display OTA progress percentage
        }

        if (ota_process_3)
        {
            LcdDisplay.print(10, 40, "OTA UP TO DATE"); // Display "SOFTWARE UPDATE" text
        }

        if (ota_process_4)
        {
            LcdDisplay.print(10, 40, "OTA CHECK FAILED"); // Display "SOFTWARE UPDATE" text
        }

        display.display();             // Update the display with the buffer content
        esp_task_wdt_reset();
        vTaskDelay(pdMS_TO_TICKS(10)); // Yield to other tasks with a delay of 10 milliseconds
    }
}

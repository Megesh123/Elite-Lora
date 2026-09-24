#include "HeaderFile.h"

void ABOUTDEVICEFUNCTION::Page()
{
    display.clearDisplay();
    LcdDisplay.Boder();

    int firstItem = scrollOffset + 1;
    int lastItem = min(scrollOffset + maxVisibleItems, totalItems);

    for (int i = firstItem; i <= lastItem; i++)
    {
        int y = (i - scrollOffset - 1) * itemHeight + 12;

        if (AboutDeviceSetcursor == i * 12)
        {
            if (VariableSeleteEnable)
            {
                display.setCursor(10, y);
                display.print(PRINT_RIGHT_ARROW);
                LcdDisplay._DrawStraightLine(7, y - 4, 5, HORIZONTAL);
            }
            else
            {
                if (LcdUI._shouldBlink(ArrowlastBlinkTime, ArrowblinkState))
                {
                    display.setCursor(10, y);
                    display.print(PRINT_RIGHT_ARROW);
                    LcdDisplay._DrawStraightLine(7, y - 4, 5, HORIZONTAL);
                }
            }
        }

        // display.setCursor(20, y);
        switch (i)
        {
        case 1:
            LcdDisplay.print(20, y, PRINT_VOLTAGE, int(VoltageMonitor.measured));
            display.print(PRINT_VOLT);
            break;

        case 2:
            if (CurrentMonitor.measured <= 0)
                CurrentMonitor.measured = 0;
            LcdDisplay.print(20, y, PRINT_CURRENT, CurrentMonitor.measured);
            display.print(PRINT_AMPS);
            break;

        case 3:
            LcdDisplay.printTime(20, y, PRINT_TOTAL_OPERATION_TIME, TotalOperationTime.Hour, TotalOperationTime.Minute);
            break;

        case 4:
            LcdDisplay.print(20, y, PRINT_TOTAL_LUBRICATION_DISCHARGE, GreaseOut.Lubrication.Total.Grams);
            break;

        case 5:
            LcdDisplay.printTime(20, y, PRINT_TOTAL_PAUSE_TIME, GreaseOut.PauseTime.Total.Hour, GreaseOut.PauseTime.Total.Minute);
            break;

        case 6:
            LcdDisplay.print(20, y, PRINT_TOTAL_ALERT, Alert.Total.NoOfAlert);
            break;

        case 7:
            LcdDisplay.print(20, y, PRINT_TOTAL_ALERT_REFILL, Alert.Total.Reservoir);
            break;

        case 8:
            LcdDisplay.print(20, y, PRINT_TOTAL_ALERT_OVERLOAD, Alert.Total.OverLoad);
            break;

        case 9:
            LcdDisplay.print(20, y, PRINT_TOTAL_ALERT_SUPERVISORY_1, Alert.Total.Supervisory1);
            break;

        case 10:
            LcdDisplay.print(20, y, PRINT_TOTAL_ALERT_SUPERVISORY_2, Alert.Total.Supervisory2);
            break;

        case 11:
            LcdDisplay.print(20, y, PRINT_TOTAL_ALERT_SUPERVISORY_3, Alert.Total.Supervisory3);
            break;

        case 12:
            LcdDisplay.print(20, y, PRINT_TOTAL_ALERT_SIGNAL_NOT_DETECTED, Alert.Total.SignalNotDetected);
            break;

        case 13:
            LcdDisplay.print(20, y, PRINT_TOTAL_ALERT_PUMPWEAK_1, Alert.Total.PumpWeak1);
            break;

        case 14:
            LcdDisplay.print(20, y, PRINT_TOTAL_ALERT_PUMPWEAK_2, Alert.Total.PumpWeak2);
            break;

        case 15:
            LcdDisplay.print(20, y, PRINT_TOTAL_ALERT_PUMPWEAK_3, Alert.Total.PumpWeak3);
            break;

        case 16:
            LcdDisplay.print(20, y, PRINT_SYSTEM);
            Admin.System();
            break;

        case 17:
            LcdDisplay.print(20, y, PRINT_FACTORY_RESET);
            Admin.FactoryReset();
            break;
        }
    }

    //_drawScrollBar();

    LcdUI._drawScrollBar(totalItems, maxVisibleItems, scrollOffset);
    LcdDisplay.Boder();
    display.display();
}

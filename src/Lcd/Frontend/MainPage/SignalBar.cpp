#include "Headerfile.h"
#include <ST7565_LCD.h>

#define BAR_WIDTH   2
#define BAR_GAP     2
#define BAR_COUNT   4
#define BAR_MAX_H   8

#define SIGNAL_X    5
#define SIGNAL_Y    3

static const uint8_t barHeights[BAR_COUNT] = {2,4,6,8};

uint8_t SignalBarFunction::getSignalLevel(int csq)
{
    if (csq >= 20) return 4;
    if (csq >= 15) return 3;
    if (csq >= 10) return 2;
    if (csq > 0)   return 1;
    return 0;
}

void SignalBarFunction::drawSignalBars(uint8_t level)
{
    uint8_t x = SIGNAL_X;
    uint8_t y = SIGNAL_Y;

    for(uint8_t i=0;i<BAR_COUNT;i++)
    {
        uint8_t barHeight = barHeights[i];

        uint8_t barX = x + i*(BAR_WIDTH + BAR_GAP);
        uint8_t barY = y + (BAR_MAX_H - barHeight);

        if(i < level)
        {
            display.fillRect(barX, barY, BAR_WIDTH, barHeight, ST7565_ON);
        }
        else
        {
            display.drawRect(barX, barY, BAR_WIDTH, barHeight, ST7565_ON);
        }
    }

    //display.display();
}



void SignalBarFunction::clearSignalArea()
{
    uint8_t totalWidth = BAR_COUNT*BAR_WIDTH + (BAR_COUNT-1)*BAR_GAP;

    display.fillRect(
        SIGNAL_X,
        SIGNAL_Y,
        totalWidth,
        BAR_MAX_H,
        ST7565_OFF
    );
}
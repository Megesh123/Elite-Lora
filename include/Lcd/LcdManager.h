/**********************
 * LCD MANAGER.H
 * Header file for LCD manager
 **********************/
#ifndef __LCD_MANAGER_H__
#define __LCD_MANAGER_H__

/**********************
 * LcdManager CLASS
 * Manages overall LCD operations
 **********************/
class LcdManager
{
public:
    void DisplayFunction(); // Main function to handle LCD display operations
};
extern LcdManager Lcd; // Global instance of LcdManager

#endif
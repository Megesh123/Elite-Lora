/**********************
 * CURRENT MONITORING HEADER
 * Defines interface for electrical current measurement and overload protection
 * Handles current sensing and overload detection for system safety
 **********************/

#ifndef __CURRENT_FUNCTION_H__
#define __CURRENT_FUNCTION_H__

/**********************
 * CURRENTFUNCTION CLASS
 * Encapsulates methods for current measurement and overload management
 **********************/
class CURRENTFUNCTION
{
public:
    void read();  // Read current sensor data
    void begin(); // Initialize current monitoring system
    float Read();

private:
    unsigned long lastReadMillis = 0;
};

extern CURRENTFUNCTION Current; // Global current monitoring instance

#endif /* __CURRENT_FUNCTION_H__ */
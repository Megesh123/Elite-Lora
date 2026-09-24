/**********************
 * VOLTAGE MONITORING HEADER
 * Defines interface for system voltage monitoring and protection
 * Handles voltage reading, conversion, and low voltage protection
 **********************/

#ifndef __VOLTAGE_FUNCTION_H__
#define __VOLTAGE_FUNCTION_H__

/**********************
 * VOLTAGEFUNCTION CLASS
 * Encapsulates methods for voltage measurement and low voltage management
 **********************/
class VOLTAGEFUNCTION
{
public:
    void read();  // Read voltage sensor data
    void begin(); // Initialize voltage monitoring system
    float Read(); // Read voltage sensor data

private:
    float floatMap(float x, float in_min, float in_max, float out_min, float out_max); // Map float values from one range to another
};
extern VOLTAGEFUNCTION Voltage;

#endif /* __VOLTAGE_FUNCTION_H__ */
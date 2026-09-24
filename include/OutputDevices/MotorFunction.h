/**********************
 * MOTOR CONTROL HEADER
 * Defines interface for pump motor control operations
 * Handles motor activation, deactivation, and safety features
 **********************/

#ifndef __MOTOR_FUNCTION__
#define __MOTOR_FUNCTION__

/**********************
 * MOTORFUNCTION CLASS
 * Manages motor control functions and safety operations
 **********************/
class MOTORFUNCTION
{
public:
    void begin();  // Initialize and start motor with proximity sensor safety checks
    void reset();  // Reset motor operation with proximity sensing integration
    void on();     // Turn motor on at full speed (PUMP_ON = 0)
    void off();    // Turn motor off completely (PUMP_OFF = 255)
    void toggle(); // Toggle motor to intermediate speed (PUMP_SPEED = 192)
};

extern MOTORFUNCTION Motor; // Global instance of MOTORFUNCTION

#endif /* __MOTOR_FUNCTION__ */
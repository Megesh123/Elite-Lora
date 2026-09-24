/**********************
 * SUPERVISORYFUNCTION CLASS
 * Manages supervisory monitoring and outlet element health checks
 **********************/
#ifndef __SUPERVISORY_H__
#define __SUPERVISORY_H__

#include <Arduino.h>

/**********************
 * SUPERVISORYFUNCTION CLASS
 * Manages supervisory monitoring and outlet element health checks
 **********************/
class SUPERVISORYFUNCTION
{
public:
    void Supervisory();             // Main supervisory monitoring function
    void Supervisory1();            // Supervisory monitoring for outlet 1
    void Supervisory2();            // Supervisory monitoring for outlet 2
    void Supervisory3();            // Supervisory monitoring for outlet 3
    void Check_Supervisory_Alert(); // Check and trigger supervisory alerts

    bool _pump_status_send_lora = false; // Flag to track if pump status has been sent
private:
    void _supervisory1_norminal(); // Supervisory 1 nominal condition check
    void _supervisory2_norminal(); // Supervisory 2 nominal condition check
    void _supervisory3_norminal(); // Supervisory 3 nominal condition check

    void _supervisory1_average(); // Supervisory 1 average condition check
    void _supervisory2_average(); // Supervisory 2 average condition check
    void _supervisory3_average(); // Supervisory 3 average condition check

    void _supervisory1_Outlet_Element_Health(); // Supervisory 1 outlet element health check
    void _supervisory2_Outlet_Element_Health(); // Supervisory 2 outlet element health check
    void _supervisory3_Outlet_Element_Health(); // Supervisory 3 outlet element health check

    bool LimitSwitch1Pressed = false;
    bool LimitSwitch2Pressed = false;
    bool LimitSwitch3Pressed = false;

    void _Supervisory1_Alert(); // Trigger alert for supervisory 1
    void _Supervisory2_Alert(); // Trigger alert for supervisory 2
    void _Supervisory3_Alert(); // Trigger alert for supervisory 3
};

extern SUPERVISORYFUNCTION Supervisory; // Global instance of SUPERVISORYFUNCTION

#endif
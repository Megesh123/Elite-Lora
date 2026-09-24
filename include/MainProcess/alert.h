/**********************
 * ALERT SYSTEM HEADER
 * Defines the alert functionality interface for the lubrication control system
 * Handles various system alerts including overload, pressure issues, and sensor faults
 **********************/

#ifndef __ALERT_H__
#define __ALERT_H__

/**********************
 * ALERTFUNCTION CLASS
 * Manages alert system operations and state
 **********************/
class ALERTFUNCTION
{
public:
    void begin();
    void overLoad();          // Trigger overload alert when current exceeds limits
    void reservoir();         // Trigger reservoir low level alert
    void proximityNotSense(); // Trigger proximity sensor not sensing alert
    void Supervisory1();      // Trigger supervisory alert 1
    void Supervisory2();      // Trigger supervisory alert 2
    void Supervisory3();      // Trigger supervisory alert 3

    void clear();   // Clear all active alerts and reset system to normal operation
    void occured(); // Main alert handler - processes active alerts

    void ResetProcess(); // Reset alert system state and variables

    void LoraPublishAlert(); // Process LoRa alert messages and trigger corresponding alerts
    void Buzzer();

private:
    void _FunctionStop();    // Stop all system functions when alert occurs
    void _AlertIdentifier(); // Identify and process LoRa alert messages

    void _identifierOverload();          // Process overload alert identification
    void _identifierReservoir();         // Process reservoir alert identification
    void _identifierProximityNotSense(); // Process proximity sensor alert identification
    void _identifierSupervisory1();      // Process proximity sensor alert identification
    void _identifierSupervisory2();      // Process proximity sensor alert identification
    void _identifierSupervisory3();      // Process proximity sensor alert identification

    void _identifierPumpElement1_Weak(); // Process pump element weak alert identification
    void _identifierPumpElement2_Weak(); // Process pump element weak alert identification
    void _identifierPumpElement3_Weak(); // Process pump element weak alert identification

    enum AlertNameCount
    {
        empty,
        _overload,
        _reservoir,
        _supervisory1,
        _supervisory2,
        _supervisory3,
        _proximity,
        _pumpweak1,
        _pumpweak2,
        _pumpweak3
    };
};

extern ALERTFUNCTION AlertFun; // Global instance of ALERTFUNCTION

// Only safe way to change Alert.Name: bounded copy under the shared state lock,
// so writers on any task/core cannot tear a concurrent reader's string.
void SetAlertName(const char *s);

#endif /* __ALERT_H__ */
/**********************
 * LED INDICATOR CONTROL HEADER
 * Defines interface for LED status indicators and visual feedback
 * Handles system status, alerts, and fault condition indications
 **********************/

#ifndef __LED_H__
#define __LED_H__

/**********************
 * LEDFUNCTION CLASS
 * Manages LED indicator functions and status patterns
 **********************/
class LEDFUNCTION
{
public:
    void off();               // Turn off all LED indicators
    void on();                // Turn on all LED indicators for full status display
    void overLoad();          // Indicate electrical overload condition
    void reservoir();         // Indicate reservoir low level condition
    void proximityNotSense(); // Indicate proximity sensor not sensing
    void Supervisory();       // Indicate supervisory or monitoring mode
    void Testrun_on();        // Activate test run mode indication
    void Testrun_off();       // Deactivate test run mode indication

    void _on();
    void _off();
    void _overload();
    void _reservoir();
    void _proximity();
    void _supervisory();
    void _testrun();
};

extern LEDFUNCTION Led; // Global instance of LEDFUNCTION

enum LedState
{
    _on,
    _off,
    _overload,
    _supervisory,
    _reservoir,
    _proximity,
    _testrun
};

#endif /* __LED_H__ */

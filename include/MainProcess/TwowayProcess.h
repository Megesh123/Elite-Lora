#ifndef __TWOWAYPROCESS_H__
#define __TWOWAYPROCESS_H__

struct TwowayProcessVariable
{
    bool TestRunOn = false;
    bool TestRunOff = false;

    bool SleepModeOn = false;
    bool SleepModeOff = false;

    bool SupervisoryAlertEnableDisable = false;
    bool RefillAlertEnableDisable = false;

    bool RefillSuccessful = false;
    String RefillSuccessful_Data = "";
    bool AlertClear = false;
    String AlertClear_Data = "";
};
extern TwowayProcessVariable Lora_twp;

class TwowayProcessFunction
{
public:
    void Process();
    void Return();
};
extern TwowayProcessFunction Twoway;
#endif
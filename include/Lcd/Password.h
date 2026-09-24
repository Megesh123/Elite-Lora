#ifndef __PASSWORD_H__
#define __PASSWORD_H__

class ADMINSETTING
{
public:
    void page();
    void FactoryReset();
    void DisplayEnterPassword();
    void DisplayWorngPassword();
    void System();

private:
    void _take_old_sate();
    bool _up();
    bool _down();
    void _blink();
    bool _back();
    bool _menu();
    void _ok();

    void _Blink_Current_Digit();
    void _System_setting();

    void _system_up();
    void _system_down();
    void _system_ok();
    void _system_back();

    void CorrectPassword(long int enteredPass);
    void WorngPassword(long int enteredPass);

    bool oldstate_Supervisory1_Enable;
    bool oldstate_Supervisory2_Enable;
    bool oldstate_Supervisory3_Enable;
    bool oldstate_Refill_Enable;
    bool check_refill_status = true;
    bool _Pressed = true;
    bool _enter_outletdischare = false;
    bool _enter_overload = false;

    unsigned long CurrentMillis = 0;
    unsigned long ButtonPressMillis = 0;
    const int ButtonInterval = 250;
    unsigned long PreviousMillis = 0;
    bool BlinkState = true;
    const int OnTime = 700;
    const int OffTime = 300;
    int CurrentDigit = 0;
};
extern ADMINSETTING Admin;

#endif
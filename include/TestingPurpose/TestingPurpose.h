#ifndef __TESTING_PURPOSE_H__
#define __TESTING_PURPOSE_H__

class TestingFunction
{
public:
    void voltageTest();
    void currentTest();
    void proximityTest();
    void limitSwitchTest();
    void Mode();
    void menuButtonTest();
    void backButtonTest();
    void upButtonTest();
    void downButtonTest();
    void okButtonTest();

private:
    bool _onces_print_volt_test = false;
    bool _onces_print_curr_test = false;
    bool _onces_print_prox_test = false;
    bool _onces_print_limit_1_sw_test = false;
    bool _onces_print_limit_2_sw_test = false;
    bool _onces_print_limit_3_sw_test = false;
    bool _onces_print_buzzer_test = false;
    bool _onces_print_menu_button_test = false;
    bool _onces_print_back_button_test = false;
    bool _onces_print_up_button_test = false;
    bool _onces_print_down_button_test = false;
    bool _onces_print_ok_button_test = false;

    bool _button_menu_pressed = false;
    bool _button_back_pressed = false;
    bool _button_up_pressed = false;
    bool _button_down_pressed = false;
    bool _button_ok_pressed = false;
};
extern TestingFunction Testing;

extern bool AlreadyTestedBoard;
extern bool TestingModeEnable;

#endif
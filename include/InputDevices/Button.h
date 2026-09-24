/**********************
 * BUTTON.H
 * Header file for button input device management
 **********************/
#ifndef __BUTTON_H__
#define __BUTTON_H__

#include <Arduino.h>
#include <functional> // For std::function

#define Pressed LOW // Change to HIGH if your buttons work differently
#define NotPressed HIGH

/**********************
 * BUTTONKEY CLASS
 * Encapsulates individual button behavior and state
 **********************/
class BUTTONKEY
{
private:
    int Keypin;                                         // Pin number for the button
    std::function<void()> pressedCallback = nullptr;    // Callback for button press
    std::function<void()> notPressedCallback = nullptr; // Callback for button release

public:
    BUTTONKEY(int pin) : Keypin(pin) {} // Constructor to initialize button pin

    bool wait(uint32_t timeoutMs = 5000); // returns true if released, false on timeout
    bool isPressed();                     // Check if button is pressed
    bool isNotPressed();                  // Check if button is not pressed
};

/**********************
 * BUTTONFUNCTION CLASS
 * Manages multiple buttons and their interactions
 **********************/
class BUTTONFUNCTION
{
public:
    BUTTONKEY menu; // Menu button
    BUTTONKEY back; // Back button
    BUTTONKEY up;   // Up button
    BUTTONKEY down; // Down button
    BUTTONKEY ok;   // OK button

    BUTTONFUNCTION(int menuPin, int backPin, int upPin, int downPin, int okPin)
        : menu(menuPin), back(backPin), up(upPin), down(downPin), ok(okPin) {} // Constructor to initialize all buttons
};

extern BUTTONFUNCTION Button; // Global button management instance

#endif
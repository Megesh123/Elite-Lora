/**
 * @file Main.cpp
 * @author Megeshwaran D <megesh@bfes.co.in>
 * @version 1.0.0
 * @date 2025-10-01
 * @copyright Copyright (c) 2025 [Blackfox Embedded Solutions]
 *
 * @note Bluetooth page frontend logic and display management.
 */

#include "Headerfile.h"
#include "Lcd/Logo/Bluetooth.h"

/**********************
 * BLUETOOTH DISPLAY FUNCTION
 * Manages Bluetooth display logic and UI updates
 **********************/
void BluetoothDisplay::Function()
{
    bluetooth.Connected = Bluetooth.CheckBluetoothConnect(); // Update connection status
    display.clearDisplay();                                  // Clear the display buffer
    Logo();                                                  // Display Bluetooth logo
    Enable();                                                // Display "Enable" status
    Enabled();                                               // Display "Enabled" status
    Connecting();                                            // Display "Connecting" status
    Connected();                                             // Display "Connected" status
    Receive();                                               // Display "Receive" status
    Received();                                              // Display "Received" status
    display.display();                                       // Update the display with the buffer content
}

/**********************
 * BLUETOOTH LOGO DISPLAY FUNCTION
 * Renders the Bluetooth logo on the display
 **********************/
void BluetoothDisplay::Logo()
{
    display.drawBitmap(0, 0, BluetoothLogo, 64, 65, 1); // Draw Bluetooth logo at (0,0) with width 64 and height 65
}

/**********************
 * BLUETOOTH ENABLE DISPLAY FUNCTION
 * Displays "Enable" status on the screen
 **********************/
void BluetoothDisplay::Enable()
{
    if (bluetooth.Enable) // If Bluetooth is enabled
    {
        LcdDisplay.print(65, 60, PRINT_ENABLE); // Print "Enable" at (70,60)
    }
}

/**********************
 * BLUETOOTH ENABLED DISPLAY FUNCTION
 * Displays "Enabled" status on the screen
 **********************/
void BluetoothDisplay::Enabled()
{
    if (bluetooth.Enabled) // If Bluetooth is enabled
    {
        LcdDisplay.print(65, 10, PRINT_ENABLED); // Print "Enabled" at (70,10)
    }
}

/**********************
 * BLUETOOTH CONNECTING DISPLAY FUNCTION
 * Displays "Connecting" status on the screen
 **********************/
void BluetoothDisplay::Connecting()
{
    if (bluetooth.Connecting) // If Bluetooth is connecting
    {
        LcdDisplay.print(65, 60, PRINT_CONNECTING); // Print "Connecting" at (70,60)
    }
}

/**********************
 * BLUETOOTH CONNECTED DISPLAY FUNCTION
 * Displays "Connected" status on the screen
 **********************/
void BluetoothDisplay::Connected()
{
    if (bluetooth.Connected) // If Bluetooth is connected
    {
        LcdDisplay.print(65, 20, PRINT_CONNECTED); // Print "Connected" at (70,20)
        bluetooth.Connecting = false;              // Reset connecting status
        bluetooth.IncomingDataReceive = true;      // Set incoming data receive flag
    }
    else
    {
        bluetooth.IncomingDataReceive = false;
        bluetooth.Connecting = true;
    }
}

/**********************
 * BLUETOOTH RECEIVE DISPLAY FUNCTION
 * Displays "Receive" status on the screen
 **********************/
void BluetoothDisplay::Receive()
{
    if (bluetooth.IncomingDataReceive) // If incoming data is being received
    {
        LcdDisplay.print(65, 60, PRINT_RECEIVE); // Print "Receive" at (70,60)
    }
}

/**********************
 * BLUETOOTH RECEIVED DISPLAY FUNCTION
 * Displays "Received" status on the screen
 **********************/
void BluetoothDisplay::Received()
{
    if (bluetooth.IncomingDataReceived) // If incoming data has been received
    {
        LcdDisplay.print(65, 60, PRINT_RECEIVED); // Print "Receive" at (70,60)
        LcdDisplay.print(65, 30, PRINT_RECEIVE);  // Print "Received" at (70,30)
    }
}
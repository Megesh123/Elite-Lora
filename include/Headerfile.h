/**********************
 * HEADERFILE H
 * Centralized header file including all necessary system and component headers
 **********************/
#ifndef __HEADER_FILE_H__
#define __HEADER_FILE_H__

/**********************
 * INCLUDED HEADERS
 * Comprehensive inclusion of all system and component headers
 * Facilitates modular design and easy access to all functionalities
 **********************/
#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include <freertos/semphr.h>
#include "freertos/task.h"
#include <freertos/queue.h>
#include "SystemFiles/System.h"
#include "MainProcess/IoT/LoraFunction.h"
#include "MainProcess/Bluetooth.h"
#include "EEPROM/eepromFunction.h"
#include "MainProcess/GreaseOutFunction.h"
#include "Task/Tasks.h"
#include "Timer/Timer.h"
#include "MCP7940.h"
#include <Wire.h>
#include "MainProcess/alert.h"
// #include "MainProcess/SerialMonitor.h"
#include "InputDevices/CurrentFunction.h"
#include "InputDevices/VoltageFunction.h"
#include "OutputDevices/MotorFunction.h"
#include "OutputDevices/Led.h"
#include <esp_flash.h>
#include <esp_err.h>
#include "EEPROM/FlashStoreFunction.H"
#include "MainProcess/Reservoir.h"
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <ST7565_LCD.h>
#include "SystemFiles/config.h"
#include "InputDevices/Button.h"
#include "LCD/Backend.h"
#include "BLEScanner.h"
// #include "SystemFiles/Logger.h"
#include "Lcd\LcdDisplay.h"
#include "Lcd\LcdManager.h"
#include "Lcd\LcdUI.h"
#include "MainProcess/Supervisory.h"
#include "Lcd/AboutDevice.h"
#include "Lcd/Password.h"
#include "Lcd/Print.h"
#include "TestingPurpose/TestingPurpose.h"
#include "OTA.h"
#include "MainProcess/TwowayProcess.h"

#endif
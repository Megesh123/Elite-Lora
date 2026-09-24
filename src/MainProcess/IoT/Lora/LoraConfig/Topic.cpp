/**
 * @file Main.cpp
 * @author Megeshwaran D <megesh@bfes.co.in>
 * @version 1.0.0
 * @date 2025-10-01
 * @copyright Copyright (c) 2025 [Blackfox Embedded Solutions]
 *
 * @note LoRa configuration for topics
 */

#include "Headerfile.h"

/**********************
 * LoRa PLAYLOAD TOPIC SETUP
 * Configures LoRa topics for payload and return messages
 **********************/
void LoRaFunction::playloadTopic()
{
  LoraDetails.Clientid = Bluetooth.getName();
  LoraTopics.subscribe = LoraDetails.Clientid + PlayLoadTopic.App;                                     // App command topic
  LoraTopics.alert = LoraDetails.Clientid + PlayLoadTopic.Alert;                                       // Alert topic
  LoraTopics.offline = LoraDetails.Clientid + PlayLoadTopic.Offline;                                   // Offline data topic
  LoraTopics.system_reset = LoraDetails.Clientid + PlayLoadTopic.FactoryReset;                         // Factory reset topic
  LoraTopics.refill = LoraDetails.Clientid + PlayLoadTopic.Refill;                                     // Refill topic
  LoraTopics.refill_mode = LoraDetails.Clientid + PlayLoadTopic.RefillMode;                            // Refill mode topic
  LoraTopics.testrun = LoraDetails.Clientid + PlayLoadTopic.TestRun;                                   // Test run topic
  LoraTopics.LiveData = LoraDetails.Clientid + PlayLoadTopic.TotalDischarge;                           // Live data topic
  LoraTopics.check_app_online = LoraDetails.Clientid + PlayLoadTopic.LiveDataUpdate;                   // App online check
  LoraTopics.SleepMode = LoraDetails.Clientid + PlayLoadTopic.SleepMode;                               // Sleep mode topic
  LoraTopics.LiveController = LoraDetails.Clientid + PlayLoadTopic.Live;                               // Live controller topic
  LoraTopics.Reconfig = LoraDetails.Clientid + PlayLoadTopic.Reconfig;                                 // Reconfiguration topic
  LoraTopics.alert_clear = LoraDetails.Clientid + PlayLoadTopic.AlertRectified;                        // Alert clear topic
  LoraTopics.Reservoircapacity = LoraDetails.Clientid + PlayLoadTopic.Tank;                            // Reservoir capacity topic
  LoraTopics.Dashboard = LoraDetails.Clientid + PlayLoadTopic.Dashboard;                               // Dashboard topic
  LoraTopics.Snooze = LoraDetails.Clientid + PlayLoadTopic.Snooze;                                     // Snooze topic
  LoraTopics.Setting = LoraDetails.Clientid + PlayLoadTopic.Setting;                                   // Setting topic
  LoraTopics.PumpStatus = LoraDetails.Clientid + PlayLoadTopic.PumpStatus;                             // Pump status topic
  LoraTopics.SupervisoryEnableDisable = LoraDetails.Clientid + PlayLoadTopic.SupervisoryEnableDisable; // Supervisory level topic
  LoraTopics.RefillAlert = LoraDetails.Clientid + PlayLoadTopic.RefillAlert;                           // Refill alert topic
  LoraTopics.Mode = LoraDetails.Clientid + PlayLoadTopic.Mode;
  LoraTopics.ConMode = LoraDetails.Clientid + PlayLoadTopic.ConMode;
  LoraTopics.LiveMode = LoraDetails.Clientid + PlayLoadTopic.Alert; // Live mode topic
}

/**********************
 * LoRa RETURN TOPIC SETUP
 * Configures LoRa topics for return/acknowledgment messages
 **********************/
void LoRaFunction::returnTopic()
{
  LoraTopics.subscribe_return = LoraDetails.Clientid + ReturnTopic.App;                                     // App acknowledgment
  LoraTopics.alert_return = LoraDetails.Clientid + ReturnTopic.Alert;                                       // Alert acknowledgment
  LoraTopics.offline_return = LoraDetails.Clientid + ReturnTopic.Offline;                                   // Offline data acknowledgment
  LoraTopics.system_reset_return = LoraDetails.Clientid + ReturnTopic.FactoryReset;                         // Factory reset acknowledgment
  LoraTopics.refill_return = LoraDetails.Clientid + ReturnTopic.Refill;                                     // Refill acknowledgment
  LoraTopics.refill_mode_return = LoraDetails.Clientid + ReturnTopic.RefillMode;                            // Refill mode acknowledgment
  LoraTopics.testrun_return = LoraDetails.Clientid + ReturnTopic.TestRun;                                   // Test run acknowledgment
  LoraTopics.SleepMode_return = LoraDetails.Clientid + ReturnTopic.SleepMode;                               // Sleep mode acknowledgment
  LoraTopics.LiveController_return = LoraDetails.Clientid + ReturnTopic.Live;                               // Live controller acknowledgment
  LoraTopics.Reconfig_return = LoraDetails.Clientid + ReturnTopic.Reconfig;                                 // Reconfiguration acknowledgment
  LoraTopics.alert_clear_return = LoraDetails.Clientid + ReturnTopic.AlertRectified;                        // Alert clear acknowledgment
  LoraTopics.Snooze_return = LoraDetails.Clientid + ReturnTopic.Snooze;                                     // Snooze acknowledgment
  LoraTopics.Setting_return = LoraDetails.Clientid + ReturnTopic.Setting;                                   // Setting acknowledgment
  LoraTopics.SupervisoryEnableDisable_return = LoraDetails.Clientid + ReturnTopic.SupervisoryEnableDisable; // Supervisory level acknowledgment topic
  LoraTopics.RefillAlert_return = LoraDetails.Clientid + ReturnTopic.RefillAlert;                           // Refill alert acknowledgment topic
  LoraTopics.Dashboard_return = LoraDetails.Clientid + ReturnTopic.Dashboard;                               // Dashboard topic
  LoraTopics.ConMode_return = LoraDetails.Clientid + ReturnTopic.ConMode;
}

// /**
//  * @file Main.cpp
//  * @author Megeshwaran D <megesh@bfes.co.in>
//  * @version 1.0.0
//  * @date 2025-10-01
//  * @copyright Copyright (c) 2025 [Blackfox Embedded Solutions]
//  *
//  * @note LOGGER IMPLEMENTATION
//  */

// #include "Headerfile.h"

// LogLevel Logger::currentLevel = LOG_INFO;
// Logger Log;

// /**********************
//  * LOGGER INITIALIZATION
//  * Sets up the logging system with specified level and baud rate
//  **********************/
// void Logger::begin(LogLevel level, unsigned long baud)
// {
//     Serial.begin(baud);                                                 // Initialize Serial communication
//     currentLevel = level;                                               // Set current log level
//     delay(100);                                                         // Short delay to ensure Serial is ready
//    // Log.info("Logger initialized (Level: %d, Baud: %lu)", level, baud); // Log initialization message
// }

// /**********************
//  * SET LOG LEVEL
//  * Updates the current logging level
//  **********************/
// void Logger::setLevel(LogLevel level)
// {
//     currentLevel = level; // Update current log level
// }

// /**********************
//  * LOGGING FUNCTION
//  * Logs messages based on the specified log level
//  **********************/
// void Logger::log(LogLevel level, const char *fmt, va_list args)
// {
//     if (level > currentLevel || level == LOG_NONE)
//         return; // Skip logging if level is too low or none

//     char buffer[512];                             // Buffer for formatted log message
//     vsnprintf(buffer, sizeof(buffer), fmt, args); // Format the log message

//     int actual_length = strlen(buffer);
//     if (actual_length >= sizeof(buffer))
//     {
//         Serial.println("[WARN] Log message truncated due to buffer size limit"); // Warn about potential truncation
//     }

//     const char *tag = ""; // Tag for log level
//     switch (level)
//     {
//     case LOG_ERROR:
//         tag = "[ERROR] "; // Error level tag
//         break;
//     case LOG_WARN:
//         tag = "[WARN]  "; // Warning level tag
//         break;
//     case LOG_INFO:
//         tag = "[INFO]  "; // Info level tag
//         break;
//     case LOG_DEBUG:
//         tag = "[DEBUG] "; // Debug level tag
//         break;
//     default:
//         break;
//     }

//     Serial.print(tag);      // Print log level tag
//     Serial.println(buffer); // Print formatted log message
// }

// /**********************
//  * LOG ERROR MESSAGE
//  * Logs an error message with variable arguments
//  **********************/
// void Logger::error(const char *fmt, ...)
// {
//     Log.setLevel(LOG_ERROR);
//     va_list args;              // Variable argument list
//     va_start(args, fmt);       // Initialize argument list
//     log(LOG_ERROR, fmt, args); // Log error message
//     va_end(args);              // Clean up argument list
// }

// /**********************
//  * LOG WARNING MESSAGE
//  * Logs a warning message with variable arguments
//  **********************/
// void Logger::warn(const char *fmt, ...)
// {
//     Log.setLevel(LOG_WARN);
//     va_list args;             // Variable argument list
//     va_start(args, fmt);      // Initialize argument list
//     log(LOG_WARN, fmt, args); // Log warning message
//     va_end(args);             // Clean up argument list
// }

// /**********************
//  * LOG INFO MESSAGE
//  * Logs an info message with variable arguments
//  **********************/
// void Logger::info(const char *fmt, ...)
// {
//     Log.setLevel(LOG_INFO);
//     va_list args;             // Variable argument list
//     va_start(args, fmt);      // Initialize argument list
//     log(LOG_INFO, fmt, args); // Log info message
//     va_end(args);             // Clean up argument list
// }

// /**********************
//  * LOG DEBUG MESSAGE
//  * Logs a debug message with variable arguments
//  **********************/
// void Logger::debug(const char *fmt, ...)
// {
//     Log.setLevel(LOG_DEBUG);
//     va_list args;              // Variable argument list
//     va_start(args, fmt);       // Initialize argument list
//     log(LOG_DEBUG, fmt, args); // Log debug message
//     va_end(args);              // Clean up argument list
// }

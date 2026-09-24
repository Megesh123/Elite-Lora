/**********************
 * LOGGER CLASS
 * Manages logging functionality with different log levels
 **********************/
#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <Arduino.h>
#include <stdarg.h>

/**********************
 * LogLevel ENUM
 * Defines various logging levels for message filtering
 **********************/
enum LogLevel
{
    LOG_NONE = 0, // No logging
    LOG_ERROR,    // Error messages only
    LOG_WARN,     // Warning messages
    LOG_INFO,     // Informational messages
    LOG_DEBUG     // Debugging messages
};

/**********************
 * Logger CLASS
 * Provides static methods for logging messages at different levels
 **********************/
class Logger
{
public:
    static void begin(LogLevel level = LOG_INFO, unsigned long baud = 115200); // Initialize logger with specified log level and baud rate
    static void setLevel(LogLevel level);                                      // Set the current logging level

    static void error(const char *fmt, ...); // Log error messages
    static void warn(const char *fmt, ...);  // Log warning messages
    static void info(const char *fmt, ...);  // Log informational messages
    static void debug(const char *fmt, ...); // Log debugging messages

private:
    static LogLevel currentLevel;                                   // Current logging level
    static void log(LogLevel level, const char *fmt, va_list args); // Internal logging method
};
extern Logger Log; // Global instance of Logger

#endif /* __LOGGER_H__ */
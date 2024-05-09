#ifndef JOJ_LOGGER_H
#define JOJ_LOGGER_H

#include "error_code.h"

#define JRELEASE 0

#if JRELEASE == 1
#define LOG_WARN_ENABLED 0
#define LOG_DEBUG_ENABLED 0
#define LOG_INFO_ENABLED 0
#else
#define LOG_WARN_ENABLED 1
#define LOG_DEBUG_ENABLED 1
#define LOG_INFO_ENABLED 1
#endif

namespace joj {
    enum LogLevel {
        LOG_LEVEL_FATAL = 0,
        LOG_LEVEL_ERROR = 1,
        LOG_LEVEL_WARN = 2,
        LOG_LEVEL_DEBUG = 3,
        LOG_LEVEL_INFO = 4,
    };

    void log_output(LogLevel level, ErrorCode err, const char* message, ...);
}

#ifndef JFATAL
#define JFATAL(error, message, ...) joj::log_output(joj::LogLevel::LOG_LEVEL_FATAL, error, message "\nFile: %s\nLine: %d", __FILE__, __LINE__, ##__VA_ARGS__);
#endif

#ifndef JERROR
#define JERROR(error, message, ...) log_output(joj::LogLevel::LOG_LEVEL_ERROR, error, message "\nFile: %s\nLine: %d", __FILE__, __LINE__, ##__VA_ARGS__);
#endif

#if LOG_WARN_ENABLED == 1
#define JWARN(message, ...) log_output(joj::LogLevel::LOG_LEVEL_WARN, joj::ErrorCode::OK, message "\nFile: %s\nLine: %d", __FILE__, __LINE__, ##__VA_ARGS__);
#else
#define JWARN(message, ...);
#endif

#if LOG_DEBUG_ENABLED == 1
#define JDEBUG(message, ...) log_output(joj::LogLevel::LOG_LEVEL_DEBUG, joj::ErrorCode::OK, message, ##__VA_ARGS__);
#else
#define JDEBUG(message, ...);
#endif

#if LOG_INFO_ENABLED == 1
#define JINFO(message, ...) log_output(joj::LogLevel::LOG_LEVEL_INFO, joj::ErrorCode::OK, message, ##__VA_ARGS__);
#else
#define JINFO(message, ...);
#endif

#endif // JOJ_LOGGER_H
